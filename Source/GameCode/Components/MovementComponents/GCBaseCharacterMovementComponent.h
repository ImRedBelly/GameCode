#pragma once

#include "CoreMinimal.h"
#include "GameCode/Actors/Interactive/Environment/Ladder.h"
#include "GameCode/Characters/GCBaseCharacter.h"
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
	CMOVE_Ladder UMETA(DisplayName "Ladder"),
	CMOVE_Max UMETA(Hidden),
};

UENUM(BlueprintType)
enum class EDetachFromLadderMethod : uint8
{
	Fall = 0,
	ReachingTheTop,
	ReachingTheDown,
	JumpOff,
};

UCLASS()
class GAMECODE_API UGCBaseCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	virtual void PhysicsRotation(float DeltaTime) override;

	FORCEINLINE bool IsSprinting() const { return bIsSprinting; }
	virtual float GetMaxSpeed() const override;

	void StartSprint();
	void StopSprint();

	void StartMantle(const FMantlingMovementParameters& MantlingParameters);
	void EndMantle();
	bool IsMantling() const;

	void AttachToLadder(const ALadder* Ladder);
	void DetachFromLadder(EDetachFromLadderMethod DetachFromLadderMethod = EDetachFromLadderMethod::Fall);
	bool IsOnLadder() const;
	const ALadder* GetCurrentLadder();
	float GetActorToCurrentLadderProjection(const FVector& Location) const;
	float GetLadderSpeedRatio() const;

protected:
	virtual void PhysCustom(float deltaTime, int32 Iterations) override;
	virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) override;
	void PhysMantling(float deltaTime, int32 Iterations);
	void PhysLadder(float deltaTime, int32 Iterations);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character movement: sprint",
		meta = (ClampMin = 0.0f, UIMin = 0.0f))
	float SprintSpeed = 1200.0f;

	UPROPERTY(Category="Character Movement: Swimming", EditAnywhere, BlueprintReadWrite, meta=(ClampMin="0", UIMin="0"))
	float SwimmingCapsuleRadius = 60.0f;
	UPROPERTY(Category="Character Movement: Swimming", EditAnywhere, BlueprintReadWrite, meta=(ClampMin="0", UIMin="0"))
	float SwimmingCapsuleHalfHeight = 60.0f;

	UPROPERTY(Category="Character Movement: Ladder", EditAnywhere, BlueprintReadWrite, meta=(ClampMin="0", UIMin="0"))
	float ClimbingOnLadderMaxSpeed = 200;
	UPROPERTY(Category="Character Movement: Ladder", EditAnywhere, BlueprintReadWrite, meta=(ClampMin="0", UIMin="0"))
	float ClimbingOnLadderBreakingDeceleration = 2048;
	UPROPERTY(Category="Character Movement: Ladder", EditAnywhere, BlueprintReadWrite, meta=(ClampMin="0", UIMin="0"))
	float LadderToCharacterOffset = 60;
	UPROPERTY(Category="Character Movement: Ladder", EditAnywhere, BlueprintReadWrite, meta=(ClampMin="0", UIMin="0"))
	float MaxLadderTopOffset = 90;
	UPROPERTY(Category="Character Movement: Ladder", EditAnywhere, BlueprintReadWrite, meta=(ClampMin="0", UIMin="0"))
	float MinLadderBottomOffset = 90;
	UPROPERTY(Category="Character Movement: Ladder", EditAnywhere, BlueprintReadWrite, meta=(ClampMin="0", UIMin="0"))
	float JumpOffFromLadderSpeed = 500;

	AGCBaseCharacter* GetBaseCharacterOwner() const;

private:
	bool bIsSprinting;
	FMantlingMovementParameters CurrentMantlingParameters;
	FTimerHandle MantlingTimer;

	const ALadder* CurrentLadder = nullptr;

	FRotator ForceTargetRotation = FRotator::ZeroRotator;
	bool bForceRotation = false;
};
