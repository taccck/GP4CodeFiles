#include "HamBillboardWidget.h"

#include "Camera/CameraComponent.h"
#include "Hamster/Characters/HamCharacter.h"
#include "Kismet/GameplayStatics.h"

void UHamBillboardWidget::BeginPlay()
{
	Super::BeginPlay();
	Height = GetRelativeLocation().Z;
}

void UHamBillboardWidget::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    	if (!IsValid(CamToFace))
    	{
    		const APawn* Player = UGameplayStatics::GetPlayerPawn(this, 0);
    		if (!IsValid(Player))
    		{
    			return;
    		}
    		
    		for (UActorComponent* Component : Player->GetComponents())
    		{
    			UCameraComponent* Cam = Cast<UCameraComponent>(Component);
    			if (IsValid(Cam))
    			{
    				CamToFace = Cam;
    				break;
    			}
    		}
    		return;
    	}
	Billboard();
}

void UHamBillboardWidget::Billboard()
{
	SetWorldRotation((CamToFace->GetComponentLocation() - GetComponentLocation()).Rotation());
	FVector Location = GetOwner()->GetActorLocation();
	Location.Z += Height;
	SetWorldLocation(Location);
}
