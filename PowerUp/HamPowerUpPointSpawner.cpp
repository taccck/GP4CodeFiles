#include "HamPowerUpPointSpawner.h"

#include "HamPowerUpPickup.h"
#include "Components/BillboardComponent.h"

AHamPowerUpPointSpawner::AHamPowerUpPointSpawner()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AHamPowerUpPointSpawner::BeginPlay()
{
	Super::BeginPlay();
	SpawnLocation = GetActorLocation();
	SpawnRotation = GetActorRotation();

	AttemptToSpawn();
	
	//AttemptToSpawn();
}

AActor* AHamPowerUpPointSpawner::AttemptToSpawn()
{
	if (SpawnedObject != nullptr)
	{
		return SpawnedObject;
	}
	AActor* SpawnedItem = GetWorld()->SpawnActor<AActor>(AbilityPickup, SpawnLocation, SpawnRotation, SpawnInfo);
	SpawnedObject = SpawnedItem;
	SpawnedThing = Cast<AHamPowerUpPickup>(SpawnedItem);
	if (SpawnedThing == nullptr)
	{
		GetWorldTimerManager().ClearTimer(MemberTimerHandle);
		return AttemptToSpawn();
	}
	SpawnedThing->Spawner = this;
	GetWorldTimerManager().ClearTimer(MemberTimerHandle);
	return SpawnedItem;

	
}

void AHamPowerUpPointSpawner::SpawnTimerStart()
{
	GetWorldTimerManager().SetTimer(MemberTimerHandle, this, &AHamPowerUpPointSpawner::RepeatingFunction, TimeToSpawn, false, TimeToSpawn);

}




void AHamPowerUpPointSpawner::RepeatingFunction()
{
	
	AttemptToSpawn();
	SpawnedObject = nullptr;
}


