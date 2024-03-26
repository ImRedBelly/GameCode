#pragma once

#include "CoreMinimal.h"
#include "AmmoWidget.h"
#include "HighlightInteractable.h"
#include "ReticleWidget.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUDWidget.generated.h"

UCLASS()
class GAMECODE_API UPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UReticleWidget* GetReticleWidget();
	UAmmoWidget* GetAmmoWidget();

	void SetHightlightInteractableVisibility(bool bIsVisible);
	void SetHightlightInteractableActionName(FName KeyName);

protected:
	UFUNCTION(BlueprintCallable)
	float GetHealthPercent() const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Widget names")
	FName ReticleWidgetName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Widget names")
	FName AmmoWidgetName;

	UPROPERTY(meta=(BindWidget))
	UHighlightInteractable* InteractableKey;
};
