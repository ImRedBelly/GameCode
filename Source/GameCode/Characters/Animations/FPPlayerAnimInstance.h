#pragma once

#include "CoreMinimal.h"
#include "GCBaseCharacterAnimInstance.h"
#include "GameCode/Characters/FPPlayerCharacter.h"
#include "FPPlayerAnimInstance.generated.h"


UCLASS()
class GAMECODE_API UFPPlayerAnimInstance : public UGCBaseCharacterAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character | Animation | FirstPerson")
	float PlayerCameraPitchAngle = 0;
 
	TWeakObjectPtr<AFPPlayerCharacter> CachedFirstPlayerCharacterOwner;

private:
	float CalculateCameraPitchAngle() const;
};
