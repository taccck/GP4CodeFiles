// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "HamPowerUpPointSpawner.generated.h"

class AHamPowerUpPickup;

UCLASS()
class HAMSTER_API AHamPowerUpPointSpawner : public ATargetPoint
{
	GENERATED_BODY()

public:
	AHamPowerUpPointSpawner();

	UFUNCTION()
	AActor* AttemptToSpawn();

	UFUNCTION()
	void SpawnTimerStart();
	


	UFUNCTION()
	void RepeatingFunction();
	
protected:
virtual void BeginPlay() override;
	
private:



public:
	UPROPERTY(EditAnywhere)
	AActor* SpawnedObject = nullptr;
	UPROPERTY(EditAnywhere)
	AHamPowerUpPickup* SpawnedThing;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AHamPowerUpPickup> AbilityPickup;
	UPROPERTY(EditAnywhere)
	float TimeToSpawn = 5.f;
	UPROPERTY(EditAnywhere)
	float TimeForFirstSpawn = 0.f;
	
protected:

	
private:
	UPROPERTY()
	FTimerHandle MemberTimerHandle;
	UPROPERTY()
	FVector SpawnLocation;
	UPROPERTY()
	FRotator SpawnRotation;
	


	FActorSpawnParameters SpawnInfo;
};
