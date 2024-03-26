#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "GCAttributeProgressBar.generated.h"


UCLASS()
class GAMECODE_API UGCAttributeProgressBar : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetProgressPercentage(float Percentage);

protected:
	UPROPERTY(meta=(BindWidget))
	UProgressBar* HealthProgressBar;
};
