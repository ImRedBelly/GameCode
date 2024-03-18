#pragma once

#include "CoreMinimal.h"
#include "GameCode/Actors/Equipment/EquipableItem.h"
#include "GameCode/Components/Weapon/WeaponBarellComponent.h"
#include "RangeWeaponItem.generated.h"

class UAnimMontage;

UCLASS(Blueprintable)
class GAMECODE_API ARangeWeaponItem : public AEquipableItem
{
	GENERATED_BODY()

public:
	ARangeWeaponItem();
	void Fire();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	USkeletalMeshComponent* WeaponMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UWeaponBarellComponent* WeaponBarell;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Animations|Weapon")
	UAnimMontage* WeaponFireMontage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Animations|Character")
	UAnimMontage* CharacterFireMontage;

private:
	float PlayAnimMontage(UAnimMontage* AnimMontage);
};
