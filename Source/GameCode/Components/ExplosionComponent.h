#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "ExplosionComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnExplosion);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAMECODE_API UExplosionComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void Explode(AController* Controller);

	UPROPERTY(BlueprintAssignable)
	FOnExplosion OnExplosion;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Explosion|Damage")
	float MaxDamage = 75;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Explosion|Damage")
	float MinDamage = 30;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Explosion|Damage")
	float DamageFalloff = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Explosion|Damage")
	TSubclassOf<UDamageType> DamageTypeClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Explosion|Raduis")
	float InnerRadius = 300;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Explosion|Raduis")
	float OuterRadius = 500;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Explosion|VFX")
	UParticleSystem* ExplosionVFX;
};
