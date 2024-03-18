#include "CharacterEquipmentComponent.h"

#include "GameCode/GameCodeTypes.h"

EEquipableItemType UCharacterEquipmentComponent::GetCurrentEquippedItemType() const
{
	EEquipableItemType Result = EEquipableItemType::None;
	if (IsValid(CurrentEquippedWeapon))
	{
		Result = CurrentEquippedWeapon->GetItemType();
	}
	return Result;
}

void UCharacterEquipmentComponent::Fire()
{
	if (IsValid(CurrentEquippedWeapon))
	{
		CurrentEquippedWeapon->Fire();
	}
}

void UCharacterEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();
	checkf(GetOwner()->IsA<AGCBaseCharacter>(),
	       TEXT(
		       "UCharacterEquipmentComponent::BeginPlay() CharacterEquipmentComponent Character can be used only with a BaseCharacter"
	       ));

	CachedBaseCharacter = StaticCast<AGCBaseCharacter*>(GetOwner());
	CreateLoadout();
}

void UCharacterEquipmentComponent::CreateLoadout()
{
	if (!IsValid(SideArmClass)) return;
	CurrentEquippedWeapon = GetWorld()->SpawnActor<ARangeWeaponItem>(SideArmClass);
	CurrentEquippedWeapon->AttachToComponent(CachedBaseCharacter->GetMesh(),
	                                         FAttachmentTransformRules::KeepRelativeTransform, SocketCharacterWeapon);
	CurrentEquippedWeapon->SetOwner(CachedBaseCharacter.Get());
}
