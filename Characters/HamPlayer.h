#pragma once
#include "HamCharacter.h"
#include "HamPlayer.generated.h"

UCLASS(Abstract)
class AHamPlayer : public AHamCharacter
{
	GENERATED_BODY()
public:
	AHamPlayer();

protected:
	virtual void BeginPlay() override;
	virtual void TickActor(float DeltaTime, ELevelTick TickType, FActorTickFunction& ThisTickFunction) override;
	UFUNCTION(BlueprintCallable)
	FRotator GetAimRotation(const FVector SpawnProjectileLocation, bool& bValid) const;
	
private:
	UFUNCTION(BlueprintCallable)
	void MoveForwardInput(float Axis);
	UFUNCTION(BlueprintCallable)
	void MoveRightInput(float Axis);

public:
	UPROPERTY(BlueprintReadWrite)
	FVector MoveInput;
	
private:
	UPROPERTY(EditDefaultsOnly)
	float AimDistance = 100000.f;
	
};
