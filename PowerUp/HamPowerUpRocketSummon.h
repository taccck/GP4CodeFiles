#pragma once

#include "GameFramework/Actor.h"
#include "Hamster/Weapons/HamRocket.h"
#include "HamPowerUpRocketSummon.generated.h"


UCLASS()
class HAMSTER_API AHamPowerUpRocketSummon : public AHamRocket
{
	GENERATED_BODY()
	
public:	
	AHamPowerUpRocketSummon();


	virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                   FVector NormalImpulse, const FHitResult& Hit) override;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	FActorSpawnParameters SpawnInfo;
	

	UPROPERTY(EditAnywhere)
	int DamageAmount = 100;
};
