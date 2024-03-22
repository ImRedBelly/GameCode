#include "EquipableItem.h"

void AEquipableItem::SetOwner(AActor* NewOwner)
{
	Super::SetOwner(NewOwner);
	if (IsValid(NewOwner))
	{
		checkf(GetOwner()->IsA<AGCBaseCharacter>(), TEXT("AEquipableItem::SetOwner() only character can be an owner of and equipable item"))
		CachedCharacterOwner = StaticCast<AGCBaseCharacter*>(GetOwner());
	}
	else
		CachedCharacterOwner = nullptr;
}

EEquipableItemType AEquipableItem::GetItemType() const
{
	return ItemType;
}

FName AEquipableItem::GetUnEquippedSocketName() const
{
	return UnEquippedSocketName;
}

FName AEquipableItem::GetEquippedSocketName() const
{
	return EquippedSocketName;
}

UAnimMontage* AEquipableItem::GetCharacterEquipAnimMontage() const
{
	return CharacterEquipAnimMontage;
}

EReticleType AEquipableItem::GetReticleType() const
{
	return ReticleType;
}

void AEquipableItem::Equip()
{
	if (OnEquipmentStateChanged.IsBound())
		OnEquipmentStateChanged.Broadcast(true);
}

void AEquipableItem::UnEquip()
{
	if (OnEquipmentStateChanged.IsBound())
		OnEquipmentStateChanged.Broadcast(false);
}

AGCBaseCharacter* AEquipableItem::GetCharacterOwner() const
{
	return CachedCharacterOwner.IsValid() ? CachedCharacterOwner.Get() : nullptr;
}
