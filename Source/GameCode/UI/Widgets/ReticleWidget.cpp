#include "ReticleWidget.h"

void UReticleWidget::OnAimingStateChanged_Implementation(bool bIsAiming)
{
	SetupCurrentReticle();
}

void UReticleWidget::OnEquippedItemChange_Implementation(const AEquipableItem* EquipableItem)
{
	CurrentEquipableItem = EquipableItem;
	SetupCurrentReticle();
}

void UReticleWidget::SetupCurrentReticle()
{
	CurrentReticle = CurrentEquipableItem.IsValid() ? CurrentEquipableItem->GetReticleType() : EReticleType::None;
}
