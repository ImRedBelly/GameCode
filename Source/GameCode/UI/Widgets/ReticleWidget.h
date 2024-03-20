#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ReticleWidget.generated.h"

UCLASS()
class GAMECODE_API UReticleWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintImplementableEvent)
	void OnAimingStateChanged(bool bIsAiming);
};
