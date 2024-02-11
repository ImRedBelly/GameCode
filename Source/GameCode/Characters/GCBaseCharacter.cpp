#include "GCBaseCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameCode/Components/MovementComponents/GCBaseCharacterMovementComponent.h"


AGCBaseCharacter::AGCBaseCharacter(const FObjectInitializer& ObjectInitializer) : Super(
	ObjectInitializer.SetDefaultSubobjectClass<UGCBaseCharacterMovementComponent>(CharacterMovementComponentName))
{
	GCBaseCharacterMovementComponent = StaticCast<UGCBaseCharacterMovementComponent*>(GetCharacterMovement());
}

void AGCBaseCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	TryChangeSprintState();
}

void AGCBaseCharacter::ChangeCrouchState()
{
	if (GetCharacterMovement()->IsCrouching())
	{
		UnCrouch();
	}
	else
	{
		Crouch();
	}
}

void AGCBaseCharacter::StartSprint()
{
	bIsSprintRequested = true;
	if (bIsCrouched)
		UnCrouch();
}

void AGCBaseCharacter::StopSprint()
{
	bIsSprintRequested = false;
}

bool AGCBaseCharacter::CanSprint()
{
	return true;
}

void AGCBaseCharacter::TryChangeSprintState()
{
	if (bIsSprintRequested && !GCBaseCharacterMovementComponent->IsSprinting() && CanSprint())
	{
		GCBaseCharacterMovementComponent->StartSprint();
		OnSprintStart();
	}
	if (!bIsSprintRequested && GCBaseCharacterMovementComponent->IsSprinting())
	{
		GCBaseCharacterMovementComponent->StopSprint();
		OnSprintEnd();
	}
}
