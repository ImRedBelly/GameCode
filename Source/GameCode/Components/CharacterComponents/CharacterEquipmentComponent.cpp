#include "CharacterEquipmentComponent.h"

#include "GameCode/GameCodeTypes.h"
#include "Net/UnrealNetwork.h"

UCharacterEquipmentComponent::UCharacterEquipmentComponent()
{
	SetIsReplicatedByDefault(true);
}

void UCharacterEquipmentComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UCharacterEquipmentComponent, CurrentEquippedSlot);
	DOREPLIFETIME(UCharacterEquipmentComponent, AmmunitionArray);
	DOREPLIFETIME(UCharacterEquipmentComponent, ItemsArray);
}

EEquipableItemType UCharacterEquipmentComponent::GetCurrentEquippedItemType() const
{
	EEquipableItemType Result = EEquipableItemType::None;
	if (IsValid(CurrentEquippedItem))
	{
		Result = CurrentEquippedItem->GetItemType();
	}
	return Result;
}

ARangeWeaponItem* UCharacterEquipmentComponent::GetCurrentRangeWeapon() const
{
	return CurrentEquippedWeapon;
}

AMeleeWeaponItem* UCharacterEquipmentComponent::GetCurrentMeleeWeapon() const
{
	return CurrentMeleeWeapon;
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
	CurrentThrowableItem = Cast<AThrowableItem>(CurrentEquippedItem);
	CurrentMeleeWeapon = Cast<AMeleeWeaponItem>(CurrentEquippedItem);

	if (IsValid(CurrentEquippedItem))
	{
		UAnimMontage* EquipMontage = CurrentEquippedItem->GetCharacterEquipAnimMontage();
		if (IsValid(EquipMontage))
		{
			bIsEquipping = true;
			float MontageDuration = CachedBaseCharacter->PlayAnimMontage(EquipMontage);
			GetWorld()->GetTimerManager().SetTimer(EquipTimer, this,
			                                       &UCharacterEquipmentComponent::EquipAnimationFinished,
			                                       MontageDuration, false);
		}
		else
		{
			AttachCurrentItemToEquippedSocket();
		}
		CurrentEquippedItem->Equip();
	}

	if (IsValid(CurrentEquippedWeapon))
	{
		OnCurrentWeaponAmmoChangedHandle = CurrentEquippedWeapon->OnAmmoChanged.AddUFunction(
			this, FName("OnCurrentWeaponAmmoChanged"));
		OnCurrentWeaponReloadedHandle = CurrentEquippedWeapon->OnReloadComplete.AddUFunction(
			this, FName("OnWeaponReloadComplete"));
		OnCurrentWeaponAmmoChanged(CurrentEquippedWeapon->GetAmmo());
	}

	if (OnEquippedItemChanged.IsBound())
		OnEquippedItemChanged.Broadcast(CurrentEquippedItem);

	CurrentEquippedSlot = Slot;
	if (GetOwner()->GetLocalRole() == ROLE_AutonomousProxy)
	{
		Server_EquipItemInSlot(CurrentEquippedSlot);
	}
}

void UCharacterEquipmentComponent::UnEquipCurrentItem()
{
	if (IsValid(CurrentEquippedItem))
	{
		CurrentEquippedItem->AttachToComponent(CachedBaseCharacter->GetMesh(),
		                                       FAttachmentTransformRules::KeepRelativeTransform,
		                                       CurrentEquippedItem->GetUnEquippedSocketName());

		CurrentEquippedItem->UnEquip();
	}
	if (IsValid(CurrentEquippedWeapon))
	{
		CurrentEquippedWeapon->StopFire();
		CurrentEquippedWeapon->StopReload(false);
		CurrentEquippedWeapon->OnAmmoChanged.Remove(OnCurrentWeaponAmmoChangedHandle);
		CurrentEquippedWeapon->OnReloadComplete.Remove(OnCurrentWeaponReloadedHandle);
		OnCurrentWeaponAmmoChanged(CurrentEquippedWeapon->GetAmmo());
	}

	PreviousEquippedSlot = CurrentEquippedSlot;
	CurrentEquippedSlot = EEquipmentSlots::None;
}

void UCharacterEquipmentComponent::AttachCurrentItemToEquippedSocket()
{
	if (IsValid(CurrentEquippedItem))
	{
		CurrentEquippedItem->AttachToComponent(CachedBaseCharacter->GetMesh(),
		                                       FAttachmentTransformRules::KeepRelativeTransform,
		                                       CurrentEquippedItem->GetEquippedSocketName());
	}
}

void UCharacterEquipmentComponent::LaunchCurrentThrowableItem()
{
	if (IsValid(CurrentThrowableItem))
	{
		CurrentThrowableItem->Throw();
		bIsEquipping = false;
		EquipItemInSlot(PreviousEquippedSlot);
	}
}

void UCharacterEquipmentComponent::EquipNextItem()
{
	if (CachedBaseCharacter->IsPlayerControlled())
	{
		if (IsSelectingWeapon())
		{
			WeaponWheelWidget->NextSegment();
		}
		else
		{
			APlayerController* PlayerController = CachedBaseCharacter->GetController<APlayerController>();
			OpenWeaponWheel(PlayerController);
		}
	}
	else
	{
		uint32 CurrentSlotIndex = (uint32)CurrentEquippedSlot;
		uint32 NextSlotIndex = NextItemsArraySlotIndex(CurrentSlotIndex);
		while (CurrentSlotIndex != NextSlotIndex
			&& (IgnoreSlotsWhileSwitching.Contains((EEquipmentSlots)NextSlotIndex)
				|| !IsValid(ItemsArray[NextSlotIndex])))
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
}

void UCharacterEquipmentComponent::EquipPreviousItem()
{
	if (CachedBaseCharacter->IsPlayerControlled())
	{
		if (IsSelectingWeapon())
		{
			WeaponWheelWidget->PreviousSegment();
		}
		else
		{
			APlayerController* PlayerController = CachedBaseCharacter->GetController<APlayerController>();
			OpenWeaponWheel(PlayerController);
		}
	}
	else
	{
		uint32 CurrentSlotIndex = (uint32)CurrentEquippedSlot;
		uint32 PreviousSlotIndex = PreviousItemsArraySlotIndex(CurrentSlotIndex);

		while (CurrentSlotIndex != PreviousSlotIndex
			&& (IgnoreSlotsWhileSwitching.Contains((EEquipmentSlots)PreviousSlotIndex)
				|| !IsValid(ItemsArray[PreviousSlotIndex])))
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
}

bool UCharacterEquipmentComponent::GetIsEquipping() const
{
	return bIsEquipping;
}

bool UCharacterEquipmentComponent::AddEquipmentItemToSlot(const TSubclassOf<AEquipableItem> EquipableItemClass,
                                                          int32 SlotIndex)
{
	if (!IsValid(EquipableItemClass))
		return false;

	AEquipableItem* DefaultItemObject = EquipableItemClass->GetDefaultObject<AEquipableItem>();

	if (!DefaultItemObject->IsSlotCompatable((EEquipmentSlots)SlotIndex))
		return false;

	if (!IsValid(ItemsArray[SlotIndex]))
	{
		AEquipableItem* Item = GetWorld()->SpawnActor<AEquipableItem>(EquipableItemClass);
		Item->AttachToComponent(CachedBaseCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform,
		                        Item->GetUnEquippedSocketName());
		Item->SetOwner(CachedBaseCharacter.Get());
		ItemsArray[SlotIndex] = Item;
	}
	else if (DefaultItemObject->IsA<ARangeWeaponItem>())
	{
		ARangeWeaponItem* RangeWeaponObject = StaticCast<ARangeWeaponItem*>(DefaultItemObject);
		int32 AmmoSlotIndex = (int32)RangeWeaponObject->GetAmmoType();
		AmmunitionArray[AmmoSlotIndex] += RangeWeaponObject->GetMaxAmmo();
	}
	return true;
}

void UCharacterEquipmentComponent::RemoveItemFromSlot(uint32 SlotIndex)
{
	if ((uint32)CurrentEquippedSlot == SlotIndex)
	{
		UnEquipCurrentItem();
	}
	ItemsArray[SlotIndex]->Destroy();
	ItemsArray[SlotIndex] = nullptr;
}

void UCharacterEquipmentComponent::OpenViewEquipment(APlayerController* PlayerController)
{
	if (!IsValid(ViewWidget))
		CreateEquipmentsWidget(PlayerController);

	if (!ViewWidget->IsVisible())
		ViewWidget->AddToViewport();
}

void UCharacterEquipmentComponent::CloseViewEquipment()
{
	if (ViewWidget->IsVisible())
		ViewWidget->RemoveFromParent();
}

bool UCharacterEquipmentComponent::IsViewVisible() const
{
	bool Result = false;
	if (IsValid(ViewWidget))
		Result = ViewWidget->IsVisible();
	return Result;
}

void UCharacterEquipmentComponent::OpenWeaponWheel(APlayerController* PlayerController)
{
	if (!IsValid(WeaponWheelWidget))
		CreateEquipmentsWidget(PlayerController);

	if (!WeaponWheelWidget->IsVisible())
		WeaponWheelWidget->AddToViewport();
}

bool UCharacterEquipmentComponent::IsSelectingWeapon() const
{
	return IsValid(WeaponWheelWidget) && WeaponWheelWidget->IsVisible();
}

void UCharacterEquipmentComponent::ConfirmWeaponSelection() const
{
	WeaponWheelWidget->ConfirmSelection();
}

void UCharacterEquipmentComponent::OnLevelDeserialized_Implementation()
{
	EquipItemInSlot(CurrentEquippedSlot);
}

const TArray<AEquipableItem*>& UCharacterEquipmentComponent::GetItems() const
{
	return ItemsArray;
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
	AutoEquip();
}

void UCharacterEquipmentComponent::CreateEquipmentsWidget(APlayerController* PlayerController)
{
	checkf(IsValid(ViewWidgetClass),
	       TEXT("UCharacterEquipmentComponent::CreateViewWidget view widget class is not defined"));
	if (!IsValid(PlayerController))
		return;

	ViewWidget = CreateWidget<UEquipmentViewWidget>(PlayerController, ViewWidgetClass);
	ViewWidget->InitializeEquipmentWidget(this);

	WeaponWheelWidget = CreateWidget<UWeaponWheelWidget>(PlayerController, WeaponWheelWidgetClass);
	WeaponWheelWidget->InitializeWeaponWheelWidget(this);
}

void UCharacterEquipmentComponent::Server_EquipItemInSlot_Implementation(EEquipmentSlots Slot)
{
	EquipItemInSlot(Slot);
}

void UCharacterEquipmentComponent::OnRep_ItemArray()
{
	for (AEquipableItem* Item : ItemsArray)
	{
		if (IsValid(Item))
		{
			Item->UnEquip();
		}
	}
}

void UCharacterEquipmentComponent::OnWeaponReloadComplete()
{
	ReloadAmmoInCurrentWeapon();
}


void UCharacterEquipmentComponent::ReloadAmmoInCurrentWeapon(int32 NumberOfAmmo, bool bCheckIsFull)
{
	int32 AvailableAmmunition = GetAvailableAmmunitionForCurrentWeapon();
	int32 CurrentAmmo = CurrentEquippedWeapon->GetAmmo();
	int32 AmmoToReload = CurrentEquippedWeapon->GetMaxAmmo() - CurrentAmmo;
	int32 ReloadedAmmo = FMath::Min(AvailableAmmunition, AmmoToReload);

	if (NumberOfAmmo > 0)
	{
		ReloadedAmmo = FMath::Min(ReloadedAmmo, NumberOfAmmo);
	}

	AmmunitionArray[(uint32)CurrentEquippedWeapon->GetAmmoType()] -= ReloadedAmmo;
	CurrentEquippedWeapon->SetAmmo(ReloadedAmmo + CurrentAmmo);

	if (bCheckIsFull)
	{
		AvailableAmmunition = AmmunitionArray[(uint32)CurrentEquippedWeapon->GetAmmoType()];
		bool bIsFullyReloaded = CurrentEquippedWeapon->GetAmmo() == CurrentEquippedWeapon->GetMaxAmmo();

		if (AvailableAmmunition == 0 || bIsFullyReloaded)
			CurrentEquippedWeapon->StopReload(true);
	}
}

void UCharacterEquipmentComponent::CreateLoadout()
{
	if (GetOwner()->GetLocalRole() < ROLE_Authority)
		return;

	AmmunitionArray.AddZeroed((uint32)EAmmunitionType::MAX);
	for (const TPair<EAmmunitionType, int32>& AmmoPair : MaxAmmunitionAmount)
	{
		AmmunitionArray[(int32)AmmoPair.Key] = FMath::Max(AmmoPair.Value, 0);
	}

	ItemsArray.AddZeroed((uint32)EEquipmentSlots::MAX);
	for (const TPair<EEquipmentSlots, TSubclassOf<AEquipableItem>>& ItemPair : ItemsLoadout)
	{
		if (!IsValid(ItemPair.Value))
		{
			continue;
		}

		AddEquipmentItemToSlot(ItemPair.Value, (int32)ItemPair.Key);
	}
}

void UCharacterEquipmentComponent::AutoEquip()
{
	if (AutoEquipItemInSlot != EEquipmentSlots::None)
	{
		EquipItemInSlot(AutoEquipItemInSlot);
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
	else
	{
		return CurrentSlotIndex + 1;
	}
}

uint32 UCharacterEquipmentComponent::PreviousItemsArraySlotIndex(uint32 CurrentSlotIndex)
{
	if (CurrentSlotIndex == 0)
	{
		return ItemsArray.Num() - 1;
	}
	else
	{
		return CurrentSlotIndex - 1;
	}
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

void UCharacterEquipmentComponent::OnRep_CurrentEquippedSlot(EEquipmentSlots CurrentEquipmentSlots_Old)
{
	EquipItemInSlot(CurrentEquippedSlot);
}
