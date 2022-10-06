#include "HamCharacter.h"

#include "EngineUtils.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/PlayerStart.h"
#include "GameFramework/SpringArmComponent.h"
#include "Hamster/PowerUp/HamPowerUp.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

AHamCharacter::AHamCharacter()
{
	Collider = CreateDefaultSubobject<USphereComponent>("Collider");
	RootComponent = Collider;
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArm->SetupAttachment(Collider);
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm);
}

void AHamCharacter::MoveInDirection(const float DeltaTime, const FVector MoveDirection, const float Yaw)
{
	if (bWaitingOnRespawn)
	{
		RespawnTimer -= DeltaTime;
		if (RespawnTimer <= 0.f)
		{
			Respawn();
		}
		return;
	}	
	AddAngularVelocity(DeltaTime, MoveDirection, Yaw);
	AddInAirVelocity(DeltaTime, MoveDirection, Yaw);
	if(HasAuthority())
	{
		SetOnGround();
		SetAirShotGround();
		CheckOutOfBounds();
	}
}

void AHamCharacter::BeginPlay()
{
	Super::BeginPlay();
	if(!GetWorld()->IsNetMode(NM_Client))
	{
		GetServerStartTransform_Implementation();
	}
	
	HealthPoints = MaxHealthPoints;
	
	AllCharacters.Add(this);
	
	AngularAcceleration = MaxAngularSpeed / SecondsToMaxAngularSpeed;
	InAirAcceleration = MaxInAirSpeed / SecondsToMaxInAirSpeed;
	MaxAngularSpeedSquared = MaxAngularSpeed * MaxAngularSpeed;
	MaxInAirSpeedSquared = MaxInAirSpeed * MaxInAirSpeed;
	ColShape = Collider->GetCollisionShape();
	OverlapShape.SetSphere(ColShape.GetSphereRadius() + 3.f);
}

void AHamCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (PowerUp != nullptr)
	{
		if (bPowerUpActivated)
		{
			PowerUp->TickActive(DeltaTime);
			if (PowerUp->ShouldDeactivate())
			{
				bPowerUpActivated = false;
				PowerUp = nullptr;
			}
		}
	}
}

	void AHamCharacter::Destroyed()
	{
		Super::Destroyed();

		AllCharacters.Remove(this);
	}

float AHamCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator,
		AActor* DamageCauser)
{
	ReceiveOnTakeDamage(); //runs on cilents
	
	const bool bValidDamageCauser = DamageCauser != this && DamageCauser != nullptr;
	if(!HasAuthority() || !bValidDamageCauser)
	{
		return Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);;
	}

	Killer = Cast<AHamCharacter>(DamageCauser);
	Killer->bHitPlayer = true;
	
	if(AirShotGround)
	{
		HealthPoints -= Damage;
	}
	else // Airshot
	{
		HealthPoints -= Damage * 2;
	}

	// Die
	if (HealthPoints <= 0.f)
	{	
		Killer->bKilledPlayer = true;
		Die();
	}
	return Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
}

	void AHamCharacter::AddAngularVelocity(const float DeltaTime, FVector MoveDirection, const float Yaw) const
	{
		/*if (bStopAngularVelocity)
		{
			BallMesh->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector, true);
			return;
		}*/

		MoveDirection = MoveDirection.RotateAngleAxis(Yaw + 90.f, FVector::UpVector).GetSafeNormal();
	
		FVector CurrentVelocity = Collider->GetPhysicsAngularVelocityInDegrees();
		const float DeltaSpeed = AngularAcceleration * DeltaTime;
		const FVector DeltaVelocity = MoveDirection * DeltaSpeed;
		ReduceVelocityToTurn(CurrentVelocity, MoveDirection, MaxAngularSpeedSquared, DeltaSpeed);
		CurrentVelocity += DeltaVelocity;
		Collider->SetPhysicsAngularVelocityInDegrees(CurrentVelocity);
	}

	void AHamCharacter::AddInAirVelocity(const float DeltaTime, FVector MoveDirection, const float Yaw) const
	{
		const bool bNoInput = MoveDirection.SizeSquared() < .01f;
		if (bNoInput || bOnGround)
		{
			return;
		}
	
		MoveDirection = MoveDirection.RotateAngleAxis(Yaw, FVector::UpVector).GetSafeNormal();

		FVector CurrentVelocity = Collider->GetPhysicsLinearVelocity();
		const float DeltaSpeed = DeltaTime * InAirAcceleration;
		const FVector DeltaVelocity = MoveDirection * DeltaSpeed;
		const float ZSpeed = CurrentVelocity.Z;
		CurrentVelocity.Z = 0.f;
		ReduceVelocityToTurn(CurrentVelocity, MoveDirection, MaxInAirSpeedSquared, DeltaSpeed);
		CurrentVelocity += DeltaVelocity;
		CurrentVelocity.Z = ZSpeed;
		Collider->SetPhysicsLinearVelocity(CurrentVelocity);
	}

	void AHamCharacter::ReduceVelocityToTurn(FVector& CurrentVelocity ,const FVector MoveDirection , const float MaxSpeedSquared,
		const float DeltaSpeed) const
	{
		if (CurrentVelocity.SquaredLength() > MaxSpeedSquared)
		{
			const float DotProduct = FVector::DotProduct(CurrentVelocity.GetSafeNormal(), MoveDirection);
			const float PercentToReduce = FMath::Clamp(1 + DotProduct,0.f,1.f);
			CurrentVelocity -= CurrentVelocity.GetSafeNormal() * (DeltaSpeed * PercentToReduce);
		}
	}

void AHamCharacter::SetOnGround()
{
	bOnGround = GetWorld()->SweepSingleByChannel(GroundHit,GetActorLocation(), GetActorLocation() + FVector::DownVector * DistanceToGround,
		FQuat::Identity, ECC_Visibility, ColShape);
}

void AHamCharacter::SetAirShotGround()
{
	AirShotGround = GetWorld()->SweepSingleByChannel(GroundHit,GetActorLocation(), GetActorLocation() + FVector::DownVector * AirShotDistance,
           		FQuat::Identity, ECC_Visibility, ColShape);
}

void AHamCharacter::CheckOutOfBounds()
{
	if(bWaitingOnRespawn)
	{
		return;
	}

	OutOfBoundsResults.Empty();
	GetWorld()->OverlapMultiByChannel(OutOfBoundsResults, GetActorLocation(),
		FQuat::Identity, ECC_WorldStatic, OverlapShape);

	for (size_t i = 0; i < OutOfBoundsResults.Num(); i++)
	{
		const bool bIsOutOfBounds = (OutOfBoundsResults[i].GetComponent() &&  OutOfBoundsResults[i].GetComponent()->ComponentHasTag("OoB"))
			|| (OutOfBoundsResults[i].GetActor() &&  OutOfBoundsResults[i].GetActor()->ActorHasTag("OoB"));
		if(bIsOutOfBounds)
		{
			Die();
		}
	}
}

void AHamCharacter::UpdateKills_Implementation()
{
	Kills++;
}

void AHamCharacter::UpdateDeath_Implementation()
{
	Deaths++;
}

void AHamCharacter::GetServerStartTransform_Implementation()
{
	TArray<APlayerStart*> AvailableStarts;
	FCollisionQueryParams OverlapInfo;
	OverlapInfo.AddIgnoredActor(this);
	
	for (TActorIterator<APlayerStart> It(GetWorld(), APlayerStart::StaticClass()); It; ++It)
	{
		const bool bBlocking = GetWorld()->OverlapAnyTestByObjectType(It->GetActorLocation(), FQuat::Identity,
			ECC_Pawn, It->GetCapsuleComponent()->GetCollisionShape(), OverlapInfo);
		if (!bBlocking)
		{
			AvailableStarts.Add(*It);
		}
	}

	UE_LOG(LogTemp, Log, TEXT("%d"), AvailableStarts.Num());
	if(AvailableStarts.Num() <= 0) 
	{
		return;
	}

	const size_t i = FMath::RandRange(0, AvailableStarts.Num() - 1);
	SetActorLocation(AvailableStarts[i]->GetActorLocation());
	SetActorRotation(AvailableStarts[i]->GetActorRotation());
}

void AHamCharacter::Respawn()
{
	bWaitingOnRespawn = false;
	HealthPoints = MaxHealthPoints;
	Killer = this;

	GetServerStartTransform_Implementation();

	ReceiveOnRespawn();
}

void AHamCharacter::Die()
{
	PowerUp = nullptr;

	//bShowKillFeed = true;
	
	//UpdateKillLogMulticast_Implementation();
	
	RespawnTimer = RespawnTime;
	bWaitingOnRespawn = true;

	UpdateDeath_Implementation();
	if(Killer != this)
	{
		Killer->UpdateKills_Implementation();
	}

	ReceiveOnDeath();
}

void AHamCharacter::UpdateKillLogMulticast_Implementation()
{
	UE_LOG(LogTemp, Log, TEXT("Change bool"));
}


void AHamCharacter::HandleActivatePowerUp(AHamCharacter* Me)
	{
	if (!bPickUpPowerup)
	{
		return;
	}
	if (PowerUp != nullptr && !bPowerUpActivated)
	{
		PowerUp->OnPowerupActivated(Me);
		bPowerUpActivated = true;
	}	}


	void AHamCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);
		DOREPLIFETIME(AHamCharacter, HealthPoints);
		DOREPLIFETIME(AHamCharacter, bHitPlayer);
		DOREPLIFETIME(AHamCharacter, bKilledPlayer);
		DOREPLIFETIME(AHamCharacter, PlayerStart)
		DOREPLIFETIME(AHamCharacter, CharacterName);
		DOREPLIFETIME(AHamCharacter, Deaths);
		DOREPLIFETIME(AHamCharacter, Kills);
		DOREPLIFETIME(AHamCharacter, bOnGround);
		DOREPLIFETIME(AHamCharacter, Killer);
		DOREPLIFETIME(AHamCharacter, bPowerupEquiped);
	}

	void AHamCharacter::EquipPowerUp(UHamPowerUp* NewPowerUp)
	{
		PowerUp = NewPowerUp;
		PowerUp->Setup();
	}

bool AHamCharacter::HasEquippedPowerUp()
{
	if (HasAuthority())
	{
		bPowerupEquiped = PowerUp != nullptr;
	}
	return bPowerupEquiped;
}
