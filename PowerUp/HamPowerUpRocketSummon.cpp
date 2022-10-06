#include "HamPowerUpRocketSummon.h"

AHamPowerUpRocketSummon::AHamPowerUpRocketSummon()
{
	
}

void AHamPowerUpRocketSummon::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	
	if(IsValid(OtherActor))
	{
		const FDamageEvent DmgEvent;
		OtherActor->TakeDamage(DamageAmount, DmgEvent, nullptr, GetOwner());
		SpawnExplosionEffect(OtherActor);
		return;
	}
	
	SpawnExplosionActor(Explosion, GetActorLocation());

	Destroy();}



void AHamPowerUpRocketSummon::BeginPlay()
{
	Super::BeginPlay();
}

void AHamPowerUpRocketSummon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
