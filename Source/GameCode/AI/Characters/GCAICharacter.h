#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BehaviorTree.h"
#include "GameCode/Characters/GCBaseCharacter.h"
#include "GameCode/Components/CharacterComponents/AIPatrollingComponent.h"
#include "GCAICharacter.generated.h"

UCLASS(Blueprintable)
class GAMECODE_API AGCAICharacter : public AGCBaseCharacter
{
	GENERATED_BODY()

public:
	AGCAICharacter(const FObjectInitializer& ObjectInitializer);
	UAIPatrollingComponent* GetPatrollingComponent() const;
	UBehaviorTree* GetBehaviorTree() const;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UAIPatrollingComponent* AIPatrollingComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	UBehaviorTree* BehaviorTree;
};
