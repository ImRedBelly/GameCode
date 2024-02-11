#include "GCBaseCharacterAnimInstance.h"
#include "GameCode/Components/MovementComponents/GCBaseCharacterMovementComponent.h"


void UGCBaseCharacterAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	checkf(TryGetPawnOwner()->IsA<AGCBaseCharacter>(),
	       TEXT(
		       "UGCBaseCharacterAnimInstance::NativeBeginPlay only game code UGCBaseCharacterAnimInstance can work with AGCBaseCharacter"
	       ));
	CachedBaseCharacter = StaticCast<AGCBaseCharacter*>(TryGetPawnOwner());
}

void UGCBaseCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (!CachedBaseCharacter.IsValid())
		return;

	UGCBaseCharacterMovementComponent* CharacterMovement = CachedBaseCharacter->GetBaseCharacterMovementComponent();

	Speed = CharacterMovement->Velocity.Size();

	bIsFalling = CharacterMovement->IsFalling();
	bIsCrouching = CharacterMovement->IsCrouching();
	bIsSprinting = CharacterMovement->IsSprinting();
	bIsSwimming = CharacterMovement->IsSwimming();
}
