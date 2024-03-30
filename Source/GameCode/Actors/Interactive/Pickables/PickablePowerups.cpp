#include "PickablePowerups.h"

#include "GameCode/GameCodeTypes.h"
#include "GameCode/Inventory/Items/InventoryItem.h"
#include "GameCode/Utils/GCDataTableUtils.h"

APickablePowerups::APickablePowerups()
{
	PowerupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PowerupMesh"));
	SetRootComponent(PowerupMesh);
}

void APickablePowerups::Interact(AGCBaseCharacter* Character)
{
	FItemTableRow* ItemData = GCDataTableUtils::FindInventoryItemData(GetDataTableID());

	if (ItemData == nullptr)
		return;

	TWeakObjectPtr<UInventoryItem> Item = TWeakObjectPtr<UInventoryItem>(NewObject<UInventoryItem>(Character, ItemData->InventoryItemClass));
	Item->Initialize(DataTableID, ItemData->InventoryItemDescription);

	const bool bPickedUp = Character->PickupItem(Item);
	if (bPickedUp)
		Destroy();
}

FName APickablePowerups::GetActionEventName() const
{
	return ActionInteract;
}
