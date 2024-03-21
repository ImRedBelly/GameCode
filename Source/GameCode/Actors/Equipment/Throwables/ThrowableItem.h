#pragma once

#include "CoreMinimal.h"
#include "GameCode/Actors/Equipment/EquipableItem.h"
#include "GameCode/Actors/Projectiles/GCProjectile.h"
#include "ThrowableItem.generated.h"

UCLASS(Blueprintable)
class GAMECODE_API AThrowableItem : public AEquipableItem
{
	GENERATED_BODY()

public:
	void Throw();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Throwables")
	TSubclassOf<AGCProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Throwables", meta = (ClampMin = -90, ClampMax = 90, UIMin = -90, UIMax = 90))
	float ThrowAngle = 0;
};
