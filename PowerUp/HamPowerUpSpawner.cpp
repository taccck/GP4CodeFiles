#include "HamPowerUpSpawner.h"
#include "HamPowerUpPickup.h"

AHamPowerUpSpawner::AHamPowerUpSpawner()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AHamPowerUpSpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

void AHamPowerUpSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHamPowerUpSpawner::SpawnAbility()
{
	if (CheckIfAbilityIsAlreadyThere())
	{
		return;
	}
	FVector SpawnLocation = FVector::Zero();
	FRotator SpawnRotation = FRotator::ZeroRotator;
	FActorSpawnParameters SpawnInfo;
	
	AActor* CurrentAbility = GetWorld()->SpawnActor<AActor>(AbilityPickup, SpawnLocation, SpawnRotation, SpawnInfo);
	CurrentAbility->SetOwner(GetOwner());
}

bool AHamPowerUpSpawner::CheckIfAbilityIsAlreadyThere()
{
	return false;
}

