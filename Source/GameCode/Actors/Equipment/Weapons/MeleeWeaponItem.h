#pragma once

#include "CoreMinimal.h"
#include "GameCode/Actors/Equipment/EquipableItem.h"
#include "GameCode/Components/Weapon/MeleeHitRegistrator.h"
#include "MeleeWeaponItem.generated.h"

USTRUCT(BlueprintType)
struct FMeleeAttackDescription
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Melee attack")
	TSubclassOf<UDamageType> DamageTypeClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Melee attack", meta=(UIMin = 0, ClampMin = 0))
	float DamageAmount = 50;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Melee attack")
	UAnimMontage* AttackMontage;
};

UCLASS(Blueprintable)
class GAMECODE_API AMeleeWeaponItem : public AEquipableItem
{
	GENERATED_BODY()

public:
	AMeleeWeaponItem();

	void StartAttack(EMeleeAttackType AttackType);
	void SetIsHitRegistrationEnabled(bool bIsRegistrationEnabled);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Melee attack")
	TMap<EMeleeAttackType, FMeleeAttackDescription> Attacks;

private:
	UFUNCTION()
	void ProcessHit(const FHitResult& HitResult, const FVector& HitDirection);
	
	TArray<UMeleeHitRegistrator*> HitRegistrators;
	TSet<AActor*> HitActors;

	FMeleeAttackDescription* CurrentAttack;
	void OnAttackTimerElapsed();
	FTimerHandle AttackTimer;
};
