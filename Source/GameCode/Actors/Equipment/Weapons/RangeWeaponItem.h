#pragma once

#include "CoreMinimal.h"
#include "GameCode/Actors/Equipment/EquipableItem.h"
#include "GameCode/Components/Weapon/WeaponBarellComponent.h"
#include "RangeWeaponItem.generated.h"

class UAnimMontage;
DECLARE_MULTICAST_DELEGATE_OneParam(FOnAmmoChanged, int32);
DECLARE_MULTICAST_DELEGATE(FOnReloadComplete);

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
	bool IsFiring() const;
	bool IsReloading() const;

	void StartAiming();
	void StopAiming();

	void StartReload();
	void StopReload(bool bIsSuccess);

	float GetAimFOV() const;
	float GetAimMovementMaxSpeed() const;

	FTransform GetForeGripTransform() const;
	int32 GetAmmo() const;
	int32 GetMaxAmmo() const;
	void SetAmmo(int32 NewAmmo);
	bool CanShoot() const;
	EAmmunitionType GetAmmoType() const;

	FOnAmmoChanged OnAmmoChanged;
	FOnReloadComplete OnReloadComplete;
	virtual EReticleType GetReticleType() const override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	USkeletalMeshComponent* WeaponMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UWeaponBarellComponent* WeaponBarell;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Animations|Weapon")
	UAnimMontage* WeaponFireMontage;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Animations|Weapon")
	UAnimMontage* WeaponReloadMontage;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Animations|Weapon")
	EReloadType ReloadType = EReloadType::FullClip;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Animations|Character")
	UAnimMontage* CharacterFireMontage;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Animations|Character")
	UAnimMontage* CharacterReloadMontage;
	
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
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon|Parameters|Ammo")
	EAmmunitionType AmmoType;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon|Parameters|Ammo", meta = (ClampMin = 1, UIMin = 1))
	int32 MaxAmmo = 30;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon|Parameters|Ammo")
	bool bAutoReload = true;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Reticle")
	EReticleType AimReticleType = EReticleType::Default;

private:
	float GetCurrentBulletSpreadAngle() const;
	void MakeShot();
	void OnShotTimerElapsed();
	float GetShotTimerInterval();
	float PlayAnimMontage(UAnimMontage* AnimMontage);
	void StopAnimMontage(UAnimMontage* AnimMontage, float BlendOutTime = 0);

	FTimerHandle ShotTimer;
	FTimerHandle ReloadTimer;
	bool bIsAiming = false;
	bool bIsFiring = false;
	bool bIsReloading = false;
	int32 Ammo;
};
