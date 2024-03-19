#pragma once

#include "CoreMinimal.h"
#include "GameCode/Actors/Equipment/EquipableItem.h"
#include "GameCode/Components/Weapon/WeaponBarellComponent.h"
#include "RangeWeaponItem.generated.h"

class UAnimMontage;

UENUM(BlueprintType)
enum class EWeaponFireMode:uint8
{
	Single,
	FullAuto
};

UCLASS(Blueprintable)
class GAMECODE_API ARangeWeaponItem : public AEquipableItem
{
	GENERATED_BODY()

public:
	ARangeWeaponItem();
	void StartFire();
	void StopFire();

	void StartAiming();
	void StopAiming();

	float GetAimFOV() const;
	float GetAimMovementMaxSpeed() const;

	FTransform GetForeGripTransform() const;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	USkeletalMeshComponent* WeaponMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UWeaponBarellComponent* WeaponBarell;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Animations|Weapon")
	UAnimMontage* WeaponFireMontage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Animations|Character")
	UAnimMontage* CharacterFireMontage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon|Parameters")
	EWeaponFireMode WeaponFireMode = EWeaponFireMode::Single;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon|Parameters", meta = (ClampMin = 1, UIMin = 1))
	float RateOfFire = 600.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon|Parameters", meta = (ClampMin = 0, ClampMax = 2, UIMin = 0, UIMax = 2))
	float SpreadAngle = 1.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon|Parameters|Aiming", meta = (ClampMin = 0, ClampMax = 2, UIMin = 0, UIMax = 2))
	float AimSpreadAngle = 0.25f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon|Parameters|Aiming", meta = (ClampMin = 0, UIMin = 0))
	float AimMovementMaxSpeed = 200;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon|Parameters|Aiming", meta = (ClampMin = 0, ClampMax = 120, UIMin = 0, UIMax = 120))
	float AimFOV = 60;

private:
	float GetCurrentBulletSpreadAngle() const;
	void MakeShot();
	FVector GetBulletSpreadOffset(float Angle, FRotator ShotRotation);
	float GetShotTimerInterval();
	float PlayAnimMontage(UAnimMontage* AnimMontage);

	FTimerHandle ShotTimer;
	bool bIsAiming = false;
};
