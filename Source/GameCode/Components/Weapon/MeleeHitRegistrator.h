#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "MeleeHitRegistrator.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnMeleeHitRegistred, const FHitResult&, const FVector&)

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAMECODE_API	UMeleeHitRegistrator : public USphereComponent
{
	GENERATED_BODY()

public:
	UMeleeHitRegistrator();

	FOnMeleeHitRegistred OnMeleeHitRegistred;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void ProcessHitRegistration();

	void SetIsHitRegistrationEnabled(bool bIsEnabled_In);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Melee hit registration")
	bool bIsHitRegistration = false;

private:
	FVector PreviousComponentLocation = FVector::ZeroVector;
};
