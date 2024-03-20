#pragma once

#include "CoreMinimal.h"
#include "NiagaraSystem.h"
#include "Components/SceneComponent.h"
#include "WeaponBarellComponent.generated.h"

USTRUCT(BlueprintType)
struct FDecalInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Decal info")
	UMaterialInterface* DecalMaterial;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Decal info")
	FVector DecalSize = FVector(5, 5, 5);
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Decal info")
	float DecalLifeTime = 10;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Decal info")
	float DecalFadeOutTime = 5;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAMECODE_API UWeaponBarellComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	void Shot(FVector ShotStart, FVector ShotDirection, AController* Controller, float SpreadAngle);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Barell attributes")
	float FiringRange = 5000.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Barell attributes", meta= (UIMin = 1, ClampMin = 1))
	int32 BulletsPerShot = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Barell attributes | Damage")
	float DamageAmount = 20.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Barell attributes | Damage")
	TSubclassOf<UDamageType> DamageTypeClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Barell attributes | VFX")
	UNiagaraSystem* MuzzleFlashFX;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Barell attributes | VFX")
	UNiagaraSystem* TraceFX;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Barell attributes | Decals")
	FDecalInfo DefaultDecalInfo;

private:
	FVector GetBulletSpreadOffset(float Angle, FRotator ShotRotation);
};
