#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GameCode/GameCodeTypes.h"
#include "GameCode/Characters/GCBaseCharacter.h"
#include "GCBaseCharacterAnimInstance.generated.h"

UCLASS()
class GAMECODE_API UGCBaseCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character animation", meta=(UIMin = 0, UIMax = 500))
	float Speed = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character animation")
	bool bIsFalling = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character animation")
	bool bIsCrouching = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character animation")
	bool bIsSprinting = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character animation")
	bool bIsSwimming = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character animation")
	bool bIsOnLadder = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character animation")
	float LadderSpeedRatio = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character animation")
	bool bIsStrafing = false;;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character animation", meta=(UIMin = -180, UIMax = 180))
	float Direction = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character animation")
	EEquipableItemType CurrentEquippedItemType = EEquipableItemType::None;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character animation")
	FRotator AimRotation = FRotator::ZeroRotator;

private:
	TWeakObjectPtr<AGCBaseCharacter> CachedBaseCharacter;
};
