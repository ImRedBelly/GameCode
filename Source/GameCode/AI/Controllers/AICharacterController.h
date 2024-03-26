#pragma once

#include "CoreMinimal.h"
#include "GCAIController.h"
#include "GameCode/AI/Characters/GCAICharacter.h"
#include "AICharacterController.generated.h"

UCLASS()
class GAMECODE_API AAICharacterController : public AGCAIController
{
	GENERATED_BODY()

public:
	virtual void SetPawn(APawn* InPawn) override;
	virtual void ActorsPerceptionUpdated(const TArray<AActor*>& UpdatedActors) override;
	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;
	void SetupPatrolling();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Movement")
	float TargetReachedRadius = 100;

private:
	TWeakObjectPtr<AGCAICharacter> CachedCharacter;

	void TryMoveToNextTarget();
	bool IsTargetReached(FVector TargetLocation) const;

	bool bIsPatrolling = false;
};
