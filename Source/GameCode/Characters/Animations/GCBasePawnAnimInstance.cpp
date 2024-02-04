#include "GCBasePawnAnimInstance.h"
#include "GameCode/Characters/GameCodeBasePawn.h"
#include "GameFramework/PawnMovementComponent.h"

void UGCBasePawnAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	checkf(TryGetPawnOwner()->IsA<AGameCodeBasePawn>(),
	       TEXT(
		       "UGCBasePawnAnimInstance::NativeBeginPlay only game code GameCodeBasePawn can work with UGCBasePawnAnimInstance"
	       ));
	CachedBasePawn = StaticCast<AGameCodeBasePawn*>(TryGetPawnOwner());
}

void UGCBasePawnAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!CachedBasePawn.IsValid())
		return;

	InputForward = CachedBasePawn->GetInputForward();
	InputRight = CachedBasePawn->GetInputRight();
	bIsInAir = CachedBasePawn->GetMovementComponent()->IsFalling();
}
