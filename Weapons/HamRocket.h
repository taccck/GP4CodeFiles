#pragma once

#include "GameFramework/Actor.h"
#include "HamRocket.generated.h"


UCLASS()
class HAMSTER_API AHamRocket : public AActor
{
	GENERATED_BODY()
	
public:	
	AHamRocket();

	UFUNCTION(BlueprintImplementableEvent)
	void SpawnExplosionEffect(AActor* TargetHit);
	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                   FVector NormalImpulse, const FHitResult& Hit);
	UFUNCTION(BlueprintCallable)
	AActor* SpawnExplosionActor(UClass* ExplosionClass, FVector Location);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;


public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> Explosion;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* Mesh;

private:
	FActorSpawnParameters SpawnInfo;
	bool bCalledHitThisFrame;
	
	UPROPERTY(EditAnywhere)
	float Impulse = 150000.f;
};
