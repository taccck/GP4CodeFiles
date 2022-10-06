#pragma once
#include "CoreMinimal.h"
#include "Hamster/Characters/HamCharacter.h"
#include "HamPowerUp.generated.h"

UCLASS(Blueprintable)
class UHamPowerUp : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	void Setup();

	UFUNCTION(BlueprintNativeEvent)
	void OnPowerupActivated(AHamCharacter* Me);

	UFUNCTION(BlueprintNativeEvent)
	bool ShouldDeactivate();

	UFUNCTION(BlueprintNativeEvent)
	void TickActive(float DeltaTime);

	UFUNCTION(Blueprintcallable)
	AHamCharacter* GetOwner() { return Cast<AHamCharacter>(GetOuter()); }

	UFUNCTION(BlueprintNativeEvent)
	FText GetPowerupName();
	
};