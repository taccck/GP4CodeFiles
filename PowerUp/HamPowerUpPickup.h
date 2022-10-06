#pragma once

#include "HamPowerUp.h"
#include "HamPowerUpPointSpawner.h"
#include "GameFramework/Actor.h"
#include "HamPowerupPickUp.generated.h"

class USphereComponent;
class URacePowerup;
class AHamPowerUpSpawner;

UCLASS()
class AHamPowerUpPickup : public AActor
{
	GENERATED_BODY()

public:
	AHamPowerUpPickup();

	UPROPERTY(EditAnywhere)
	AHamPowerUpPointSpawner* Spawner;

	UFUNCTION(BlueprintImplementableEvent)
	void PickupAudio();

	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* Sphere;

	UFUNCTION()
	void HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 BodyIndex, bool bFromSweep, const FHitResult& SweepHit);
	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<UHamPowerUp>> PowerupClasses;
};


