#pragma once

#include "CoreMinimal.h"
#include "NiagaraSystem.h"
#include "Components/SceneComponent.h"
#include "WeaponBarellComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAMECODE_API UWeaponBarellComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	void Shot(FVector ShotStart, FVector ShotDirection, AController* Controller);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Barell attributes")
	float FireRange = 5000;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Barell attributes|Damage")
	float DamageAmount = 20;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Barell attributes|VFX")
	UNiagaraSystem* MuzzleFlashFX;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Barell attributes|VFX")
	UNiagaraSystem* TraceFX;
};
