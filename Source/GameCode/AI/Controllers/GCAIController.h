#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AISense.h"
#include "GCAIController.generated.h"

UCLASS()
class GAMECODE_API AGCAIController : public AAIController
{
	GENERATED_BODY()

public:
	AGCAIController();

protected:
	AActor* GetClosestSensedActor(TSubclassOf<UAISense> SenseClass) const;
};
