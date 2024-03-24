#pragma once

#include "CoreMinimal.h"
#include "GCAIController.h"
#include "GCAICharacterController.generated.h"

class AGCAICharacter;

UCLASS()
class GAMECODE_API AGCAICharacterController : public AGCAIController
{
	GENERATED_BODY()

public:
	virtual void SetPawn(APawn* InPawn) override;
	virtual void ActorsPerceptionUpdated(const TArray<AActor*>& UpdatedActors) override;
	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Movement")
	float TargetReachedRadius = 100;

	virtual void BeginPlay() override;

private:
	TWeakObjectPtr<AGCAICharacter> CachedCharacter;

	void TryMoveToNextTarget();
	bool IsTargetReached(FVector TargetLocation) const;

	bool bIsPatrolling = false;
};
