#include "HamPlayer.h"

#include "Camera/CameraComponent.h"

AHamPlayer::AHamPlayer()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AHamPlayer::BeginPlay()
{
	Super::BeginPlay();
}

void AHamPlayer::TickActor(float DeltaTime, ELevelTick TickType, FActorTickFunction& ThisTickFunction)
{	
	Super::TickActor(DeltaTime, TickType, ThisTickFunction);
	if (!IsValid(Controller))
	{
		return;
	}
	MoveInDirection(DeltaTime, MoveInput, Controller->GetControlRotation().Yaw);
}

FRotator AHamPlayer::GetAimRotation(const FVector SpawnProjectileLocation, bool& bValid) const
{
	FHitResult Hit;
	const FVector CameraLocation = Camera->GetComponentLocation();
	const FVector AimLocation = CameraLocation + Camera->GetForwardVector() * AimDistance;
	GetWorld()->LineTraceSingleByChannel(Hit, CameraLocation,
	AimLocation, ECC_Visibility);
	bValid = Hit.bBlockingHit;
	return(Hit.ImpactPoint - SpawnProjectileLocation).Rotation();
}

void AHamPlayer::MoveForwardInput(const float Axis)
{
	MoveInput.X = Axis;
}

void AHamPlayer::MoveRightInput(const float Axis)
{
	MoveInput.Y = Axis;
}
