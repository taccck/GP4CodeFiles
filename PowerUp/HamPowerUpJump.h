#pragma once

#include "HamPowerUp.h"
#include "HamPowerUpJump.generated.h"

class AHamCharacter;

UCLASS()
class UHamPowerUpJump : public UHamPowerUp
{
	GENERATED_BODY()

public:
	void OnPowerupActivated_Implementation(AHamCharacter* Me) override;

	UPROPERTY(EditAnywhere)
	float Force = 5000.f;
	
private:
	UPROPERTY(EditDefaultsOnly)
	UTexture2D* Icon;

	UPROPERTY(EditDefaultsOnly)
	FText PowerUpName;

	UPROPERTY()
	AHamCharacter* HamCharacter;

	float BoostTime;
	

};