#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "HighlightInteractable.generated.h"

UCLASS()
class GAMECODE_API UHighlightInteractable : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetActionText(FName KeyName);

protected:
	UPROPERTY(meta=(BindWidget))
	UTextBlock* ActionText;
};
