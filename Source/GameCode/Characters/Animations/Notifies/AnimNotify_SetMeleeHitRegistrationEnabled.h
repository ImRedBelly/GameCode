#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_SetMeleeHitRegistrationEnabled.generated.h"

UCLASS()
class GAMECODE_API UAnimNotify_SetMeleeHitRegistrationEnabled : public UAnimNotify
{
	GENERATED_BODY()

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Melee weapon")
	bool bIsHitRegistrationEnabled;
};
