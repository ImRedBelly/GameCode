#include "AnimNotify_SetMeleeHitRegistrationEnabled.h"
#include "GameCode/Characters/GCBaseCharacter.h"
#include "GameCode/Components/CharacterComponents/CharacterEquipmentComponent.h"

void UAnimNotify_SetMeleeHitRegistrationEnabled::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	AGCBaseCharacter* CharacterOwner = Cast<AGCBaseCharacter>(MeshComp->GetOwner());
	if (!IsValid(CharacterOwner))
		return;

	AMeleeWeaponItem* MeleeWeapon = CharacterOwner->GetCharacterEquipmentComponent()->GetCurrentMeleeWeapon();
	if (IsValid(MeleeWeapon))
		MeleeWeapon->SetIsHitRegistrationEnabled(bIsHitRegistrationEnabled );
}
