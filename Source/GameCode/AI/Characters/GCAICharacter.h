#pragma once

#include "CoreMinimal.h"
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

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UAIPatrollingComponent* AIPatrollingComponent;
};
