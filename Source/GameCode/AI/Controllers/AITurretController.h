#pragma once

#include "CoreMinimal.h"
#include "GCAIController.h"
#include "GameCode/AI/Characters/Turret.h"
#include "AITurretController.generated.h"

UCLASS()
class GAMECODE_API AAITurretController : public AGCAIController
{
	GENERATED_BODY()

public:
	virtual void SetPawn(APawn* InPawn) override;

	virtual void ActorsPerceptionUpdated(const TArray<AActor*>& UpdatedActors) override;

private:
	TWeakObjectPtr<ATurret> CachedTurret;
};
