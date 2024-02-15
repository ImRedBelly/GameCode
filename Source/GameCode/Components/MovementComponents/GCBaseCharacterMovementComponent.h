#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameCode/Components/LedgeDetectorComponent.h"
#include "GCBaseCharacterMovementComponent.generated.h"

struct FMantlingMovementParameters
{
	FVector InitialLocation = FVector::ZeroVector;
	FRotator InitialRotation = FRotator::ZeroRotator;

	FVector TargetLocation = FVector::ZeroVector;
	FRotator TargetRotation = FRotator::ZeroRotator;
	FVector InitialAnimationLocation = FVector::ZeroVector;

	float Duration = 1.0f;
	float StartTime = 0.0f;
	UCurveVector* MantlingCurve;
};

UENUM(BlueprintType)
enum class ECustomMovementMode :uint8
{
	CMOVE_None = 0 UMETA(DisplayName "None"),
	CMOVE_Mantling UMETA(DisplayName "Mantling"),
	CMOVE_Max UMETA(Hidden),
};

UCLASS()
class GAMECODE_API UGCBaseCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	FORCEINLINE bool IsSprinting() const { return bIsSprinting; }
	virtual float GetMaxSpeed() const override;

	void StartSprint();
	void StopSprint();

	void StartMantle(const FMantlingMovementParameters& MantlingParameters);
	void EndMantle();
	bool IsMantling() const;

protected:
	virtual void PhysCustom(float deltaTime, int32 Iterations) override;
	virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character movement: sprint",
		meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float SprintSpeed = 1200.0f;

	UPROPERTY(Category="Character Movement: Swimming", EditAnywhere, BlueprintReadWrite, meta=(ClampMin="0", UIMin="0"))
	float SwimmingCapsuleRadius = 60.0f;
	UPROPERTY(Category="Character Movement: Swimming", EditAnywhere, BlueprintReadWrite, meta=(ClampMin="0", UIMin="0"))
	float SwimmingCapsuleHalfHeight = 60.0f;

private:
	bool bIsSprinting;
	FMantlingMovementParameters CurrentMantlingMovementParameters;
	FTimerHandle MantlingTimer;
};
