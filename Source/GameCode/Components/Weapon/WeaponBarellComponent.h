#pragma once

#include "CoreMinimal.h"
#include "NiagaraSystem.h"
#include "Components/SceneComponent.h"
#include "GameCode/Actors/Projectiles/GCProjectile.h"
#include "WeaponBarellComponent.generated.h"

UENUM(BlueprintType)
enum class EHitRegistrationType : uint8
{
	HitScan,
	Projectile,
};

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

USTRUCT(BlueprintType)
struct FShotInfo
{
	GENERATED_BODY()

	FShotInfo() : Location_Mul_10(FVector_NetQuantize100::ZeroVector), Direction(FVector::ZeroVector)
	{
	};

	FShotInfo(FVector Location, FVector Direction) : Location_Mul_10(Location * 10.0f), Direction(Direction)
	{
	};

	UPROPERTY()
	FVector_NetQuantize100 Location_Mul_10;

	UPROPERTY()
	FVector_NetQuantizeNormal Direction;

	FVector GetLocation() const { return Location_Mul_10 * 0.1f; }
	FVector GetDirection() const { return Direction; }
};

class AGCProjectile;
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAMECODE_API UWeaponBarellComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	UWeaponBarellComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void Shot(FVector ShotStart, FVector ShotDirection, float SpreadAngle);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Barell attributes")
	float FiringRange = 5000.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Barell attributes", meta= (UIMin = 1, ClampMin = 1))
	int32 BulletsPerShot = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Barell attributes | Hit registration")
	EHitRegistrationType HitRegistration = EHitRegistrationType::HitScan;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Barell attributes | Hit registration",
		meta= (UIMin = 1, ClampMin = 1))
	int32 ProjectilePoolSize = 10;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Barell attributes | Hit registration",
		meta=(EditCondition= "HitRegistration == EHitRegistrationType::Projectile"))
	TSubclassOf<AGCProjectile> ProjectileClass;

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

	virtual void BeginPlay() override;

private:
	void ShotInternal(const TArray<FShotInfo>& ShotsInfo);

	UFUNCTION(Server, Reliable)
	void Server_Shot(const TArray<FShotInfo>& ShotsInfo);

	UPROPERTY(ReplicatedUsing=OnRep_LastShotsInfo)
	TArray<FShotInfo> LastShotsInfo;

	UPROPERTY(Replicated)
	TArray<AGCProjectile*> ProjectilePool;

	UPROPERTY(Replicated)
	int32 CurrentProjectileIndex;

	UFUNCTION()
	void OnRep_LastShotsInfo();

	UFUNCTION()
	void ProcessProjectileHit(AGCProjectile* Projectile, const FHitResult& Hit, const FVector& Direction);

	UFUNCTION()
	void ProcessHit(const FHitResult& Hit, const FVector& Direction);
	bool HitScan(FVector ShotStart, OUT FVector& ShotEnd, FVector ShotDirection);
	void LaunchProjectile(const FVector& LaunchStart, const FVector& LaunchDirection);

	FVector GetBulletSpreadOffset(float Angle, FRotator ShotRotation) const;
	const FVector ProjectilePoolLocation = FVector(0, 0, -100);
	APawn* GetOwningPawn() const;
	AController* GetController() const;
};
