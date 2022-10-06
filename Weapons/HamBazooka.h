#pragma once

#include "Components/StaticMeshComponent.h"
#include "HamBazooka.generated.h"

class AHamRocket;
class AHamCharacter;

UCLASS()
class HAMSTER_API UHamBazooka : public UStaticMeshComponent
{
	GENERATED_BODY()

public:
	UHamBazooka();
	UFUNCTION(BlueprintCallable)
	void Shoot(const FRotator AltRotation = FRotator::ZeroRotator, const bool bUseAltRotation = false);
	UFUNCTION(BlueprintImplementableEvent)
	void JustShot();
	
protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	void ShootingTimer(float DeltaTime);
	
public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AHamRocket> Rocket;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float RocketSpawnPoint = 75.f;

private:
	FActorSpawnParameters SpawnInfo;
	TWeakObjectPtr<AHamCharacter> Owner;
	float CharacterRadius;
	float ShootingCooldownTimer = 0;

	UPROPERTY(EditAnywhere)
	float ShootingCooldown = 0.2f;
};