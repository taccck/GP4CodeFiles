#pragma once

#include "HamPowerUp.h"
#include "HamPowerUpRocket.generated.h"

class AHamCharacter;
class AHamRocket;

UCLASS()
class UHamPowerUpRocket : public UHamPowerUp
{
	GENERATED_BODY()

public:

	void OnPowerupActivated_Implementation(AHamCharacter* Me) override;
	
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<AHamRocket> RocketClass;

private:
	UPROPERTY(EditDefaultsOnly)
	UTexture2D* Icon;

	UPROPERTY(EditDefaultsOnly)
	FText PowerUpName;

	UPROPERTY()
	AHamCharacter* HamCharacter;
	
	

};