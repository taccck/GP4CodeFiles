#pragma once

#include "GameFramework/GameModeBase.h"
#include "HamsterGameModeBase.generated.h"

class AHamCharacter;

UCLASS()
class HAMSTER_API AHamsterGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	AHamsterGameModeBase();
	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AHamCharacter> AiClass;
};
