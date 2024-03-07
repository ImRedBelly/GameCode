#include "AnimNotify_EnableRagdoll.h"

#include "GameCode/GameCodeTypes.h"

void UAnimNotify_EnableRagdoll::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	MeshComp->SetCollisionProfileName(CollisionProfilePawnRagdoll);
	MeshComp->SetSimulatePhysics(true);
}
