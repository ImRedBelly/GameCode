#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameCode/GameCodeTypes.h"
#include "GameCode/Components/CharacterComponents/CharacterEquipmentComponent.h"
#include "WeaponWheelWidget.generated.h"


class UImage;
class UTextBlock;
struct FWeaponTableRow;
class UCharacterEquipmentComponent;

UCLASS()
class GAMECODE_API UWeaponWheelWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	
	void InitializeWeaponWheelWidget(UCharacterEquipmentComponent* EquipmentComponent);

	void NextSegment();
	void PreviousSegment();
	void ConfirmSelection();

protected:
	UPROPERTY(meta=(BindWidget))
	UImage* RadialBackground;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* WeaponNameText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon wheel settings")
	TArray<EEquipmentSlots> EquipmentSlotsSegments;

	virtual void NativeConstruct() override;
	void SelectSegment();

private:
	FWeaponTableRow* GetTableRowForSegment(int32 SegmentIndex) const;

	int32 CurrentSegmentIndex;
	UMaterialInstanceDynamic* BackgroundMaterial;
	TWeakObjectPtr<UCharacterEquipmentComponent> LinkedEquipmentComponent;
};
