#include "HamGameState.h"

#include "EngineUtils.h"
#include "Hamster/Characters/HamCharacter.h"
#include "Net/UnrealNetwork.h"

AHamGameState::AHamGameState()
{
	PrimaryActorTick.bCanEverTick = true;
	GameState = this;
}

void AHamGameState::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	ReceiveOnTick(DeltaSeconds);
	if (GetWorld()->IsNetMode(NM_Client))
	{
		return;
	}
	
	if (StartTimer > 0.f)
	{
		StartTimer -= DeltaSeconds;
		return;
	}

	for (TActorIterator<AHamCharacter> It(GetWorld(), AHamCharacter::StaticClass()); It; ++It)
	{
		It->Collider->SetSimulatePhysics(true);
		It->bCanShoot = true;
	}
}

void AHamGameState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AHamGameState, StartTimer);
}
