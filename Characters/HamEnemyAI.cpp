#include "HamEnemyAI.h"

void AHamEnemyAI::BeginPlay()
{
	Super::BeginPlay();
	YawOffset = FMath::RandBool()? 40 : -40;
	YawOffset += FMath::RandRange(-10.f, 10.f);
}

void AHamEnemyAI::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if(!Target.IsValid())
	{
		PickTarget();
	}
	
	BehaviourChangeTimer(DeltaSeconds);
	ShootTimer(DeltaSeconds);
	RocketJumpTimer(DeltaSeconds);
	
	Movement(DeltaSeconds);
}

// Picks closest target
void AHamEnemyAI::PickTarget()
{
	const FVector TargetLocation = GetActorLocation();
	
	float TargetIndex = 0;
	float SmallestDistance = 90000000;
	
	for(int i = 0; i < AllCharacters.Num(); i++)
	{
		if(!AllCharacters[i].IsValid() || AllCharacters[i] == this)
		{
			continue;
		}
		
		FVector CurrentTargetLocation = AllCharacters[i]->GetActorLocation();
		const float CurrentDistance = FVector::Distance(CurrentTargetLocation, TargetLocation);
		
		if(CurrentDistance < SmallestDistance)
		{
			SmallestDistance = CurrentDistance;
			TargetIndex = i;
		}
	}

	Target = AllCharacters[TargetIndex];
}

void AHamEnemyAI::ChangeBehaviour()
{
	YawOffset = -YawOffset;
	RocketJump = true;
	PickTarget();
}



// #################################################################
// Timers
// #################################################################
void AHamEnemyAI::BehaviourChangeTimer(float DeltaTime)
{
	if(BehaviourChangeCooldown <= 0)
	{
		ChangeBehaviour();
		BehaviourChangeCooldown = FMath::RandRange(10.f, 30.f);
	}

	BehaviourChangeCooldown -= DeltaTime;
}

void AHamEnemyAI::ShootTimer(float DeltaTime)
{
	if(ShootCooldown <= 0)
	{
		if(!Target.IsValid())
		{
			PickTarget();
		}
		
		Shoot();
		ShootCooldown = FMath::RandRange(.5f, 2.f);
	}
	
	ShootCooldown -= DeltaTime;
}

void AHamEnemyAI::RocketJumpTimer(float DeltaTime)
{
	if(RocketJumpCooldown <= 0 && bOnGround)
	{
		RocketJump = true;
		RocketJumpCooldown = FMath::RandRange(0.5f, 10.f);
	}
	
	RocketJumpCooldown -= DeltaTime;
}



// #################################################################
// Movement and Aiming
// #################################################################
FVector AHamEnemyAI::DirectionToTargetVector() const
{
	if(!Target.IsValid())
	{
		return FVector::ZeroVector;
	}
	
	FVector Result = Target->GetActorLocation() - GetActorLocation();
	Result.Z = 0;
	return Result.GetSafeNormal();
}

FRotator AHamEnemyAI::DirectionToTargetRotator() const
{
	if(!Target.IsValid())
	{
		return FRotator::ZeroRotator;
	}

	// Distance calculations
	const float Distance = FVector::Distance(Target->GetActorLocation(), GetActorLocation());
	const float DistanceModifier = FMath::Clamp(Distance * DistanceAlternation, 1.f, 2.f);
	
	// Random alteration for location prediction
	const float RandomPredictionOffset =
		FMath::RandRange(.5f * DistanceModifier, 1.5f * DistanceModifier) * PredictionOffset;

	// Distance alteration
	const float PredictionWithDistance = DistanceModifier * RandomPredictionOffset;
	
	const FVector TargetVelocity = Target->Collider->GetComponentVelocity() * PredictionWithDistance;
	const FVector TargetsPredictedLocation = Target->GetActorLocation() + TargetVelocity;
	const FRotator Result = (TargetsPredictedLocation - GetActorLocation()).Rotation();
	
	return Result;
}

void AHamEnemyAI::Movement(const float DeltaTime)
{
	const FVector Direction = DirectionToTargetVector();
	MoveInDirection(DeltaTime, Direction, YawOffset);
}
