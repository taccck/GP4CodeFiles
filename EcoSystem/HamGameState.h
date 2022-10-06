#pragma once
#include "GameFramework/GameStateBase.h"

#include "HamGameState.generated.h"

UCLASS(abstract)
class AHamGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	AHamGameState();
	
	UFUNCTION(BlueprintPure, meta = (DisplayName = "HamGameState"))
	static AHamGameState* Get() {return GameState;}
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnTick"))
	void ReceiveOnTick(float DeltaTime);

protected:
	virtual void Tick(float DeltaSeconds) override;

public:
	UPROPERTY(BlueprintReadOnly, Replicated)
	float StartTimer = 9.f;
	
private:
	inline static AHamGameState* GameState = nullptr;
};
