#pragma once

#include "CoreMinimal.h"
#include "GameCode/Actors/Interactive/Interface/IInteractable.h"
#include "GameFramework/Actor.h"
#include "PickableItem.generated.h"

UCLASS(Abstract, NotBlueprintable)
class GAMECODE_API APickableItem : public AActor, public IInteractable
{
	GENERATED_BODY()

public:
	const FName& GetDataTableID() const;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item")
	FName DataTableID = NAME_None;
};
