#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameCode/GameCodeTypes.h"
#include "GameCode/Actors/Equipment/Throwables/ThrowableItem.h"
#include "GameCode/Actors/Equipment/Weapons/MeleeWeaponItem.h"
#include "GameCode/Actors/Equipment/Weapons/RangeWeaponItem.h"
#include "GameCode/Characters/GCBaseCharacter.h"
#include "CharacterEquipmentComponent.generated.h"

typedef TArray<AEquipableItem*, TInlineAllocator<(uint32)EEquipmentSlots::MAX>> TItemsArray;
typedef TArray<uint32, TInlineAllocator<(uint32)EAmmunitionType::MAX>> TAmmunitionArray;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnEquippedItemChanged, const AEquipableItem*);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnCurrentWeaponAmmoChanged, int32, int32);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAMECODE_API UCharacterEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	EEquipableItemType GetCurrentEquippedItemType() const;
	ARangeWeaponItem* GetCurrentRangeWeapon() const;
	AMeleeWeaponItem* GetCurrentMeleeWeapon() const;
	void ReloadCurrenWeapon();
	void ReloadAmmoInCurrentWeapon(int32 NumberOfAmmo = 0, bool bCheckIsFull = false);

	FOnCurrentWeaponAmmoChanged OnCurrentWeaponAmmoChangedEvent;
	FOnEquippedItemChanged OnEquippedItemChanged;

	void EquipItemInSlot(EEquipmentSlots Slot);
	void UnEquipCurrentItem();
	void AttachCurrentItemToEquippedSocket();
	void LaunchCurrentThrowableItem();
	void EquipNextItem();
	void EquipPreviousItem();
	bool GetIsEquipping() const;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Loadout")
	TMap<EAmmunitionType, int32> MaxAmmunitionAmount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Loadout")
	TMap<EEquipmentSlots, TSubclassOf<AEquipableItem>> ItemsLoadout;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Loadout")
	TSet<EEquipmentSlots> IgnoreSlotsWhileSwitching;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Loadout")
	EEquipmentSlots AutoEquipItemInSlot = EEquipmentSlots::None;

private:
	TAmmunitionArray AmmunitionArray;
	TItemsArray ItemsArray;

	UFUNCTION()
	void OnWeaponReloadComplete();

	void CreateLoadout();
	void AutoEquip();
	void EquipAnimationFinished();

	uint32 NextItemsArraySlotIndex(uint32 CurrentSlotIndex);
	uint32 PreviousItemsArraySlotIndex(uint32 CurrentSlotIndex);
	int32 GetAvailableAmmunitionForCurrentWeapon();

	UFUNCTION()
	void OnCurrentWeaponAmmoChanged(int32 NewAmmo);

	EEquipmentSlots PreviousEquippedSlot;
	EEquipmentSlots CurrentEquippedSlot;

	AEquipableItem* CurrentEquippedItem;
	ARangeWeaponItem* CurrentEquippedWeapon;
	AThrowableItem* CurrentThrowableItem;
	AMeleeWeaponItem* CurrentMeleeWeapon;

	TWeakObjectPtr<AGCBaseCharacter> CachedBaseCharacter;

	FDelegateHandle OnCurrentWeaponAmmoChangedHandle;
	FDelegateHandle OnCurrentWeaponReloadedHandle;

	bool bIsEquipping = false;
	FTimerHandle EquipTimer;
};
