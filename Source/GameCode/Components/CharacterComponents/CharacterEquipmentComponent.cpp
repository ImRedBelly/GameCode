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

ARangeWeaponItem* UCharacterEquipmentComponent::GetCurrentRangeWeapon() const
{
	return CurrentEquippedWeapon;
}

void UCharacterEquipmentComponent::ReloadCurrenWeapon()
{
	check(IsValid(CurrentEquippedWeapon));
	int32 AvailableAmmunition = GetAvailableAmmunitionForCurrentWeapon();
	if (AvailableAmmunition <= 0)
		return;

	CurrentEquippedWeapon->StartReload();
}


void UCharacterEquipmentComponent::EquipItemInSlot(EEquipmentSlots Slot)
{
	if (bIsEquipping)
		return;

	UnEquipCurrentItem();

	CurrentEquippedItem = ItemsArray[(uint32)Slot];
	CurrentEquippedWeapon = Cast<ARangeWeaponItem>(CurrentEquippedItem);
	if (IsValid(CurrentEquippedItem))
	{
		UAnimMontage* EquipMontage = CurrentEquippedItem->GetCharacterEquipAnimMontage();
		if (IsValid(EquipMontage))
		{
			bIsEquipping = true;
			float MontageDuration = CachedBaseCharacter->PlayAnimMontage(EquipMontage);
			GetWorld()->GetTimerManager().SetTimer(EquipTimer, this, &UCharacterEquipmentComponent::EquipAnimationFinished, MontageDuration, false);
		}
		else
		{
			AttachCurrentItemToEquippedSocket();
		}
		CurrentEquippedSlot = Slot;
	}

	if (IsValid(CurrentEquippedWeapon))
	{
		OnCurrentWeaponAmmoChangedHandle = CurrentEquippedWeapon->OnAmmoChanged.AddUFunction(this, FName("OnCurrentWeaponAmmoChanged"));
		OnCurrentWeaponReloadedHandle = CurrentEquippedWeapon->OnReloadComplete.AddUFunction(this, FName("OnWeaponReloadComplete"));
		OnCurrentWeaponAmmoChanged(CurrentEquippedWeapon->GetAmmo());
	}
}

void UCharacterEquipmentComponent::UnEquipCurrentItem()
{
	if (IsValid(CurrentEquippedItem))
	{
		CurrentEquippedItem->AttachToComponent(CachedBaseCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform,
		                                       CurrentEquippedItem->GetUnEquippedSocketName());
	}
	if (IsValid(CurrentEquippedWeapon))
	{
		CurrentEquippedWeapon->StopFire();
		CurrentEquippedWeapon->StopReload(false);
		CurrentEquippedWeapon->OnAmmoChanged.Remove(OnCurrentWeaponAmmoChangedHandle);
		CurrentEquippedWeapon->OnReloadComplete.Remove(OnCurrentWeaponReloadedHandle);
		OnCurrentWeaponAmmoChanged(CurrentEquippedWeapon->GetAmmo());
	}

	CurrentEquippedSlot = EEquipmentSlots::None;
}

void UCharacterEquipmentComponent::AttachCurrentItemToEquippedSocket()
{
	CurrentEquippedItem->AttachToComponent(CachedBaseCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform,
	                                       CurrentEquippedItem->GetEquippedSocketName());
}

void UCharacterEquipmentComponent::EquipNextItem()
{
	uint32 CurrentSlotIndex = (uint32)CurrentEquippedSlot;
	uint32 NextSlotIndex = NextItemsArraySlotIndex(CurrentSlotIndex);

	while (CurrentSlotIndex != NextSlotIndex && !IsValid(ItemsArray[NextSlotIndex]))
	{
		NextSlotIndex = NextItemsArraySlotIndex(NextSlotIndex);

		if (NextSlotIndex == ItemsArray.Num())
		{
			NextSlotIndex = 0;
		}
	}
	if (CurrentSlotIndex != NextSlotIndex)
	{
		EquipItemInSlot((EEquipmentSlots)NextSlotIndex);
	}
}

void UCharacterEquipmentComponent::EquipPreviousItem()
{
	uint32 CurrentSlotIndex = (uint32)CurrentEquippedSlot;
	uint32 PreviousSlotIndex = PreviousItemsArraySlotIndex(CurrentSlotIndex);

	while (CurrentSlotIndex != PreviousSlotIndex && !IsValid(ItemsArray[PreviousSlotIndex]))
	{
		PreviousSlotIndex = PreviousItemsArraySlotIndex(PreviousSlotIndex);

		if (PreviousSlotIndex == ItemsArray.Num())
		{
			PreviousSlotIndex = 0;
		}
	}
	if (CurrentSlotIndex != PreviousSlotIndex)
	{
		EquipItemInSlot((EEquipmentSlots)PreviousSlotIndex);
	}
}

bool UCharacterEquipmentComponent::GetIsEquipping() const
{
	return bIsEquipping;
}

void UCharacterEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();
	checkf(GetOwner()->IsA<AGCBaseCharacter>(),
	       TEXT( "UCharacterEquipmentComponent::BeginPlay() CharacterEquipmentComponent Character can be used only with a BaseCharacter"));

	CachedBaseCharacter = StaticCast<AGCBaseCharacter*>(GetOwner());
	CreateLoadout();
}

void UCharacterEquipmentComponent::OnWeaponReloadComplete()
{
	int32 AvailableAmmunition = GetAvailableAmmunitionForCurrentWeapon();
	int32 CurrentAmmo = CurrentEquippedWeapon->GetAmmo();
	int32 AmmoToReload = CurrentEquippedWeapon->GetMaxAmmo() - CurrentAmmo;
	int32 ReloadedAmmo = FMath::Min(AvailableAmmunition, AmmoToReload);
	AmmunitionArray[(uint32)CurrentEquippedWeapon->GetAmmoType()] -= ReloadedAmmo;
	CurrentEquippedWeapon->SetAmmo(ReloadedAmmo + CurrentAmmo);
}

void UCharacterEquipmentComponent::CreateLoadout()
{
	AmmunitionArray.AddZeroed((uint32)EAmmunitionType::MAX);
	for (const TPair<EAmmunitionType, int32>& AmmoPair : MaxAmmunitionAmount)
	{
		AmmunitionArray[(int32)AmmoPair.Key] = FMath::Max(AmmoPair.Value, 0);
	}
	ItemsArray.AddZeroed((uint32)EEquipmentSlots::MAX);
	for (TPair<EEquipmentSlots, TSubclassOf<AEquipableItem>>& ItemPair : ItemsLoadout)
	{
		if (!IsValid(ItemPair.Value))
			continue;

		AEquipableItem* Item = GetWorld()->SpawnActor<AEquipableItem>(ItemPair.Value);
		Item->AttachToComponent(CachedBaseCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, Item->GetUnEquippedSocketName());
		Item->SetOwner(CachedBaseCharacter.Get());
		ItemsArray[(uint32)ItemPair.Key] = Item;
	}
}

void UCharacterEquipmentComponent::EquipAnimationFinished()
{
	bIsEquipping = false;
	AttachCurrentItemToEquippedSocket();
}

uint32 UCharacterEquipmentComponent::NextItemsArraySlotIndex(uint32 CurrentSlotIndex)
{
	if (CurrentSlotIndex == ItemsArray.Num() - 1)
	{
		return 0;
	}
	return CurrentSlotIndex + 1;
}

uint32 UCharacterEquipmentComponent::PreviousItemsArraySlotIndex(uint32 CurrentSlotIndex)
{
	if (CurrentSlotIndex == 0)
	{
		return ItemsArray.Num() - 1;
	}
	return CurrentSlotIndex - 1;
}

int32 UCharacterEquipmentComponent::GetAvailableAmmunitionForCurrentWeapon()
{
	check(GetCurrentRangeWeapon());
	return AmmunitionArray[(uint32)GetCurrentRangeWeapon()->GetAmmoType()];
}

void UCharacterEquipmentComponent::OnCurrentWeaponAmmoChanged(int32 NewAmmo)
{
	if (OnCurrentWeaponAmmoChangedEvent.IsBound())
		OnCurrentWeaponAmmoChangedEvent.Broadcast(NewAmmo, GetAvailableAmmunitionForCurrentWeapon());
}
