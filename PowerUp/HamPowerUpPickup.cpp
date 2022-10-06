#include "HamPowerUpPickup.h"

#include "HamPowerUpSpawner.h"
#include "Components/SphereComponent.h"
#include "Hamster/Characters/HamCharacter.h"

AHamPowerUpPickup::AHamPowerUpPickup()
{
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(Root);
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AHamPowerUpPickup::HandleOverlap);}

void AHamPowerUpPickup::HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 BodyIndex, bool bFromSweep, const FHitResult& SweepHit)
{

	int RandomIndex = FMath::RandRange(0, PowerupClasses.Num() - 1);
	auto PowerupClass = PowerupClasses[RandomIndex];
	
	AHamCharacter* HamChar = Cast<AHamCharacter>(OtherActor);
	if (HamChar == nullptr)
	{
		OtherActor->Destroy();
		return;
	}
	if (!HamChar->bPickUpPowerup)
	{
		return;
	}
	if (!HamChar->HasEquippedPowerUp())
	{
		HamChar->EquipPowerUp(NewObject<UHamPowerUp>(HamChar, PowerupClass));
		PickupAudio();
		if (Spawner != nullptr)
		{
			Spawner->SpawnedObject = nullptr;
			Spawner->SpawnedThing = nullptr;
			Spawner->SpawnTimerStart();
			
		}

		Spawner = nullptr;
		Destroy();	
	}}

