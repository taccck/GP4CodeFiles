#include "HamPowerUpJump.h"
#include "Hamster/Characters/HamCharacter.h"

void UHamPowerUpJump::OnPowerupActivated_Implementation(AHamCharacter* Me)
{
	HamCharacter = Me;
	FVector MoveUp = FVector::UpVector;
	MoveUp.Z = Force;
	HamCharacter->Collider->AddImpulse(MoveUp,NAME_None, true);
}