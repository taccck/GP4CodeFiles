#include "HamBazooka.h"

#include "HamRocket.h"
#include "Camera/CameraComponent.h"
#include "Hamster/Characters/HamCharacter.h"

UHamBazooka::UHamBazooka()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
}

void UHamBazooka::BeginPlay()
{
	Super::BeginPlay();
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	Owner = Cast<AHamCharacter>(GetOwner());
	CharacterRadius = Owner->Collider->GetScaledSphereRadius();
}

void UHamBazooka::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	ShootingTimer(DeltaTime);
}

void UHamBazooka::ShootingTimer(float DeltaTime)
{
	if(ShootingCooldownTimer < 0)
	{
		return;
	}

	ShootingCooldownTimer -= DeltaTime;
}


void UHamBazooka::Shoot(const FRotator AltRotation, const bool bUseAltRotation)
{
	if(ShootingCooldownTimer > 0 || Owner->bWaitingOnRespawn)
	{
		return;
	}
	ShootingCooldownTimer = ShootingCooldown;
	JustShot();
	
	const FVector BazookaLocation = Owner->GetActorLocation() + (GetForwardVector() * CharacterRadius);
	const FVector SpawnLocation = BazookaLocation + GetForwardVector() * RocketSpawnPoint;
	FHitResult Hit;
	GetWorld()->LineTraceSingleByChannel(Hit, BazookaLocation,
		SpawnLocation, ECC_Visibility);
	
	if(Hit.bBlockingHit)
	{
		GetWorld()->SpawnActor<AActor>(Rocket.GetDefaultObject()->Explosion, Hit.ImpactPoint,
			GetComponentRotation(), SpawnInfo);
	}

	

	FRotator SpawnRotation = GetComponentRotation();
	if (bUseAltRotation)
	{
		SpawnRotation = AltRotation;
	}
	
	AActor* CurrentRocket = GetWorld()->SpawnActor<AActor>(Rocket, SpawnLocation, SpawnRotation, SpawnInfo);
	CurrentRocket->SetOwner(GetOwner());
}
