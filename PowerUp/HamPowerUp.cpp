#include "HamPowerUp.h"

void UHamPowerUp::Setup_Implementation() { } 
void UHamPowerUp::OnPowerupActivated_Implementation(AHamCharacter* Me) { } 
bool UHamPowerUp::ShouldDeactivate_Implementation() { return true; }
void UHamPowerUp::TickActive_Implementation(float DeltaTime) { }

FText UHamPowerUp::GetPowerupName_Implementation() { return {}; }