#pragma once
#include "HamCharacter.h"
#include "HamEnemyAI.generated.h"

UCLASS()
class AHamEnemyAI : public AHamCharacter
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	
	UFUNCTION(BlueprintImplementableEvent)
	void Shoot();
	
private:
	void PickTarget();
	void ChangeBehaviour();
	void BehaviourChangeTimer(float DeltaTime);
	void ShootTimer(float DeltaTime);
	void RocketJumpTimer(float DeltaTime);
	void Movement(const float DeltaTime);

	UFUNCTION(BlueprintCallable)
	FVector DirectionToTargetVector() const;
	UFUNCTION(BlueprintCallable)
	FRotator DirectionToTargetRotator() const;

protected:
	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<AHamCharacter> Target;
	UPROPERTY(BlueprintReadWrite)
	bool RocketJump = false;

private:
	float YawOffset = 0; // Sets in cpp file
	float BehaviourChangeCooldown = 0; // Sets in cpp file
	float ShootCooldown = 0; // Sets in cpp file
	float RocketJumpCooldown = 0; // Sets in cpp file
	float PredictionOffset = 0.1;
	float DistanceAlternation = 0.001;
	
};
