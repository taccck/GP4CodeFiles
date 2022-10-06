#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HamPowerUpSpawner.generated.h"

class AHamPowerUpPickup;

UCLASS()
class HAMSTER_API AHamPowerUpSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	AHamPowerUpSpawner();
	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void SpawnAbility();

	UFUNCTION()
	bool CheckIfAbilityIsAlreadyThere();
	
protected:
	virtual void BeginPlay() override;

private:
	
	
public:	
	UPROPERTY(EditAnywhere)
	TSubclassOf<AHamPowerUpPickup> AbilityPickup;

protected:


private:

	UPROPERTY(EditAnywhere)
	float TimeInBetweenSpawn = 1.f;
	UPROPERTY(EditAnywhere)
	bool StopSpawning = false;
	
	
};
