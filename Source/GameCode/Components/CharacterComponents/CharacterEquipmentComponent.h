#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameCode/GameCodeTypes.h"
#include "GameCode/Actors/Equipment/Throwables/ThrowableItem.h"
#include "GameCode/Actors/Equipment/Weapons/MeleeWeaponItem.h"
#include "GameCode/Actors/Equipment/Weapons/RangeWeaponItem.h"
#include "GameCode/Characters/GCBaseCharacter.h"
#include "GameCode/UI/Widgets/Equipment/EquipmentViewWidget.h"
#include "GameCode/UI/Widgets/Equipment/WeaponWheelWidget.h"
#include "CharacterEquipmentComponent.generated.h"

class UEquipmentViewWidget;
class UWeaponWheelWidget;

typedef TArray<AEquipableItem*, TInlineAllocator<(uint32)EEquipmentSlots::MAX>> TItemsArray;
typedef TArray<uint32, TInlineAllocator<(uint32)EAmmunitionType::MAX>> TAmmunitionArray;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnEquippedItemChanged, const AEquipableItem*);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnCurrentWeaponAmmoChanged, int32, int32);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAMECODE_API UCharacterEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCharacterEquipmentComponent();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

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

	bool AddEquipmentItemToSlot(const TSubclassOf<AEquipableItem> EquipableItemClass, int32 SlotIndex);
	void RemoveItemFromSlot(uint32 SlotIndex);

	void OpenViewEquipment(APlayerController* PlayerController);
	void CloseViewEquipment();
	bool IsViewVisible() const;

	void OpenWeaponWheel(APlayerController* PlayerController);
	bool IsSelectingWeapon() const;
	void ConfirmWeaponSelection() const;

	const TArray<AEquipableItem*>& GetItems() const;

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="View")
	TSubclassOf<UEquipmentViewWidget> ViewWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="View")
	TSubclassOf<UWeaponWheelWidget> WeaponWheelWidgetClass;


	void CreateEquipmentsWidget(APlayerController* PlayerController);

private:
	UFUNCTION(Server, Reliable)
	void Server_EquipItemInSlot(EEquipmentSlots Slot);

	UPROPERTY(Replicated)
	TArray<int32> AmmunitionArray;
	UPROPERTY(ReplicatedUsing=OnRep_ItemArray)
	TArray<AEquipableItem*> ItemsArray;

	UFUNCTION()
	void OnRep_ItemArray();

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

	UPROPERTY(ReplicatedUsing=OnRep_CurrentEquippedSlot)
	EEquipmentSlots CurrentEquippedSlot;

	UFUNCTION()
	void OnRep_CurrentEquippedSlot(EEquipmentSlots CurrentEquipmentSlots_Old);

	AEquipableItem* CurrentEquippedItem;
	ARangeWeaponItem* CurrentEquippedWeapon;
	AThrowableItem* CurrentThrowableItem;
	AMeleeWeaponItem* CurrentMeleeWeapon;

	TWeakObjectPtr<AGCBaseCharacter> CachedBaseCharacter;

	FDelegateHandle OnCurrentWeaponAmmoChangedHandle;
	FDelegateHandle OnCurrentWeaponReloadedHandle;

	bool bIsEquipping = false;
	FTimerHandle EquipTimer;

	UEquipmentViewWidget* ViewWidget;
	UWeaponWheelWidget* WeaponWheelWidget;
};
