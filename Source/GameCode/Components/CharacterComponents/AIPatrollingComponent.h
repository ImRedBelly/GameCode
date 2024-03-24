#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameCode/Actors/Navifation/PatrollingPath.h"
#include "AIPatrollingComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAMECODE_API UAIPatrollingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	bool CanPatrol() const;
	
	FVector SelectClosestWayPoint();
	FVector SelectNextWayPoint();
	
protected:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category="Path")
	APatrollingPath* PatrollingPath;

private:
	int32 CurrentWayPointIndex = -1;
};
