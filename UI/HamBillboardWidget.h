#pragma once

#include "Components/WidgetComponent.h"
#include "HamBillboardWidget.generated.h"

class UCameraComponent;

UCLASS(Blueprintable, ClassGroup="UserInterface", hidecategories=(Object,Activation,"Components|Activation",Sockets,Base,Lighting,LOD,Mesh), editinlinenew, meta=(BlueprintSpawnableComponent))
class HAMSTER_API UHamBillboardWidget : public UWidgetComponent
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	void Billboard();
	
private:
	UCameraComponent* CamToFace;
	float Height;
};
