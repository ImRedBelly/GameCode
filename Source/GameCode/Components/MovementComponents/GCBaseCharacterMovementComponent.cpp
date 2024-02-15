#include "GCBaseCharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Curves/CurveVector.h"
#include "GameFramework/Character.h"

float UGCBaseCharacterMovementComponent::GetMaxSpeed() const
{
	float Result = Super::GetMaxSpeed();
	if (bIsSprinting)
		Result = SprintSpeed;
	return Result;
}

void UGCBaseCharacterMovementComponent::StartSprint()
{
	bIsSprinting = true;
	bForceMaxAccel = 1;
}

void UGCBaseCharacterMovementComponent::StopSprint()
{
	bIsSprinting = false;
	bForceMaxAccel = 0;
}

void UGCBaseCharacterMovementComponent::StartMantle(const FMantlingMovementParameters& MantlingParameters)
{
	CurrentMantlingMovementParameters = MantlingParameters;
	SetMovementMode(MOVE_Custom, (uint8)ECustomMovementMode::CMOVE_Mantling);
}

void UGCBaseCharacterMovementComponent::EndMantle()
{
	SetMovementMode(MOVE_Walking);
}

bool UGCBaseCharacterMovementComponent::IsMantling() const
{
	return UpdatedComponent && MovementMode == MOVE_Custom && CustomMovementMode ==
		(uint8)ECustomMovementMode::CMOVE_Mantling;
}

void UGCBaseCharacterMovementComponent::PhysCustom(float deltaTime, int32 Iterations)
{
	switch (CustomMovementMode)
	{
	case (uint8)ECustomMovementMode::CMOVE_Mantling:
		float ElapsedTime = GetWorld()->GetTimerManager().GetTimerElapsed(MantlingTimer) +
			CurrentMantlingMovementParameters.StartTime;
		FVector MantlingCurveValue = CurrentMantlingMovementParameters.MantlingCurve->GetVectorValue(ElapsedTime);
		float PositionAlpha = MantlingCurveValue.X;
		float XYCorrectionAlpha = MantlingCurveValue.Y;
		float ZCorrectionAlpha = MantlingCurveValue.Z;

		FVector CorrectedInitialLocation = FMath::Lerp(CurrentMantlingMovementParameters.InitialLocation,
		                                               CurrentMantlingMovementParameters.InitialAnimationLocation,
		                                               XYCorrectionAlpha);
		CorrectedInitialLocation.Z = FMath::Lerp(CurrentMantlingMovementParameters.InitialLocation.Z,
		                                         CurrentMantlingMovementParameters.InitialAnimationLocation.Z,
		                                         ZCorrectionAlpha);

		FVector NewLocation = FMath::Lerp(CorrectedInitialLocation,
		                                  CurrentMantlingMovementParameters.TargetLocation, PositionAlpha);
		FRotator NewRotation = FMath::Lerp(CurrentMantlingMovementParameters.InitialRotation,
		                                   CurrentMantlingMovementParameters.TargetRotation, PositionAlpha);

		FVector Delta = NewLocation - GetActorLocation();
		FHitResult Hit;
		SafeMoveUpdatedComponent(Delta, NewRotation, false, Hit);
		break;
	}

	Super::PhysCustom(deltaTime, Iterations);
}

void UGCBaseCharacterMovementComponent::OnMovementModeChanged(EMovementMode PreviousMovementMode,
                                                              uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PreviousMovementMode, PreviousCustomMode);
	if (MovementMode == MOVE_Swimming)
	{
		CharacterOwner->GetCapsuleComponent()->SetCapsuleSize(SwimmingCapsuleRadius, SwimmingCapsuleHalfHeight);
	}
	else if (PreviousMovementMode == MOVE_Swimming)
	{
		ACharacter* DefaultCharacter = CharacterOwner->GetClass()->GetDefaultObject<ACharacter>();
		CharacterOwner->GetCapsuleComponent()->SetCapsuleSize(
			DefaultCharacter->GetCapsuleComponent()->GetUnscaledCapsuleRadius(),
			DefaultCharacter->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight(),
			true);
	}
	if (MovementMode == MOVE_Custom)
	{
		switch (CustomMovementMode)
		{
		case (uint8)ECustomMovementMode::CMOVE_Mantling:
			GetWorld()->GetTimerManager().SetTimer(MantlingTimer, this, &UGCBaseCharacterMovementComponent::EndMantle,
			                                       CurrentMantlingMovementParameters.Duration, false);

			break;
		default:
			break;
		}
	}
}
