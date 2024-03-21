#pragma once

#include "CoreMinimal.h"
#include "GCProjectile.h"
#include "GameCode/Components/ExplosionComponent.h"
#include "ExplosiveProjectile.generated.h"

UCLASS()
class GAMECODE_API AExplosiveProjectile : public AGCProjectile
{
	GENERATED_BODY()

public:
	AExplosiveProjectile();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UExplosionComponent* ExplosionComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Explosion")
	float DetonationTime = 2;

	virtual void OnProjectileLaunched() override;

private:
	FTimerHandle DetonationTimer;
	void OnDetonationTimerElapsed();
	AController* GetController();
};
