#include "FPPlayerAnimInstance.h"

#include "GameCode/Components/MovementComponents/GCBaseCharacterMovementComponent.h"

void UFPPlayerAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	checkf(TryGetPawnOwner()->IsA<AFPPlayerCharacter>(),
	       TEXT("UFPPlayerAnimInstance::NativeBeginPlay() only FPPlayerCharacter can use UFPPlayerAnimInstance"));
	CachedFirstPlayerCharacterOwner = StaticCast<AFPPlayerCharacter*>(TryGetPawnOwner());
}

void UFPPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (!CachedFirstPlayerCharacterOwner.IsValid())
		return;

	PlayerCameraPitchAngle = CalculateCameraPitchAngle();
}

float UFPPlayerAnimInstance::CalculateCameraPitchAngle() const
{
	float Result = 0;

	AGCPlayerController* Controller = CachedFirstPlayerCharacterOwner->GetController<AGCPlayerController>();
	if (IsValid(Controller) && !Controller->GetIsIgnoreCameraPitch())
	{
		Result = Controller->GetControlRotation().Pitch;
	}

	return Result;
}
