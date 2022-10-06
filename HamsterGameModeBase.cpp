#include "HamsterGameModeBase.h"

#include "Characters/HamCharacter.h"
#include "EcoSystem/HamGameInstance.h"
#include "GameFramework/GameSession.h"
#include "Kismet/GameplayStatics.h"

AHamsterGameModeBase::AHamsterGameModeBase()
{
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;
}

void AHamsterGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	
	const UHamGameInstance* GameInst = UHamGameInstance::Get();
	if (!GameInst->bUseBots)
	{
		return;
	}
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	const int32 EmptyPlayerSlots = GameInst->PlayerCount - GetNumPlayers();
	for (size_t i = 0; i < EmptyPlayerSlots; i++)
	{
		AHamCharacter* NewAi = GetWorld()->SpawnActor<AHamCharacter>(AiClass, SpawnParams);
		NewAi->SpawnDefaultController();
	}
}
