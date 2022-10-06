#include "HamPowerUpRocket.h"
#include "Hamster/Weapons/HamRocket.h"
#include "Hamster/Characters/HamCharacter.h"
#include "Hamster/Weapons/HamBazooka.h"


void UHamPowerUpRocket::OnPowerupActivated_Implementation(AHamCharacter* Me)
{
	HamCharacter = Me;

	// FTransform SpawnTransform = GetOwner()->GetActorTransform();
	// FVector spawn = SpawnTransform.GetLocation();
	// spawn.Z += 100.f ;
	// SpawnTransform.SetLocation(spawn);
	// FActorSpawnParameters Params;
	// Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	// AActor* RocketYeet = GetWorld()->SpawnActor(RocketClass, &SpawnTransform, Params);
	// RocketYeet->SetOwner(Me);


	
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	auto Bazooka = Me->FindComponentByClass<UHamBazooka>();
	float RocketSpawnPoint = 75.f;
	const FVector BazookaLocation = Me->GetActorLocation() + (Bazooka->GetForwardVector() * Me->Collider->GetScaledSphereRadius());
	const FVector SpawnLocation = BazookaLocation + Bazooka->GetForwardVector() * RocketSpawnPoint;
	FHitResult Hit;
	GetWorld()->LineTraceSingleByChannel(Hit, BazookaLocation,
		SpawnLocation, ECC_Visibility);
	
	if(Hit.bBlockingHit)
	{
		GetWorld()->SpawnActor<AActor>(RocketClass.GetDefaultObject()->Explosion, Hit.ImpactPoint,
			Bazooka->GetComponentRotation(), SpawnInfo);
	}

	

	FRotator SpawnRotation = Bazooka->GetComponentRotation();
	
	AActor* CurrentRocket = GetWorld()->SpawnActor<AActor>(RocketClass, SpawnLocation, SpawnRotation, SpawnInfo);
	CurrentRocket->SetOwner(GetOwner());


}
