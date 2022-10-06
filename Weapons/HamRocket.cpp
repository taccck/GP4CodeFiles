#include "HamRocket.h"

#include "Hamster/EcoSystem/HamGameInstance.h"
#include "Kismet/GameplayStatics.h"

AHamRocket::AHamRocket()
{
 	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Rocket"));
	RootComponent = Mesh;

	Mesh->OnComponentHit.AddDynamic(this, &AHamRocket::OnHit);
}

void AHamRocket::BeginPlay()
{
	Super::BeginPlay();
	Mesh->AddImpulse(Mesh->GetForwardVector() * Impulse);
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
}

void AHamRocket::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if(GetWorld()->IsNetMode(NM_Client) || bCalledHitThisFrame)
	{
		return;
	}
	bCalledHitThisFrame = true;
	
	if(!IsValid(OtherActor))
	{
		SpawnExplosionActor(Explosion, GetActorLocation());
		Destroy();
		return;
	}
	
	SpawnExplosionEffect(OtherActor);
	const FDamageEvent DmgEvent;
	OtherActor->TakeDamage(1, DmgEvent, nullptr, GetOwner());
}

AActor* AHamRocket::SpawnExplosionActor(UClass* ExplosionClass, FVector Location)
{
	return GetWorld()->SpawnActor<AActor>(ExplosionClass, Location, FRotator::ZeroRotator, SpawnInfo);
}

void AHamRocket::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	bCalledHitThisFrame = false;
}

