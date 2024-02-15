#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LedgeDetectorComponent.generated.h"

USTRUCT(BlueprintType)
struct FLedgeDescription
{
	GENERATED_BODY()
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Ledge description")
	FVector Location;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Ledge description")
	FVector LedgeNormal;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Ledge description")
	FRotator Rotation;
	
};


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAMECODE_API ULedgeDetectorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	bool DetectLedge(OUT FLedgeDescription& LedgeDescription);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Detection settings", meta=(UIMin=0.0f, ClampMin = 0.0f))
	float MinimumLedgeHeight = 40.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Detection settings", meta=(UIMin=0.0f, ClampMin = 0.0f))
	float MaximumLedgeHeight = 200.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Detection settings", meta=(UIMin=0.0f, ClampMin = 0.0f))
	float ForwardCheckDistance = 100.0f;

protected:
	virtual void BeginPlay() override;

private:
	TWeakObjectPtr<ACharacter> CachedCharacterOwner;
};
