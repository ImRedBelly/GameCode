#pragma once

#include "CoreMinimal.h"
#include "GameCode/AI/Characters/GCAICharacter.h"
#include "GameFramework/Actor.h"
#include "AICharacterSpawner.generated.h"

UCLASS()
class GAMECODE_API AAICharacterSpawner : public AActor
{
	GENERATED_BODY()

public:
	AAICharacterSpawner();
	UFUNCTION()
	void SpawnAI();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="AI Spawner")
	TSubclassOf<AGCAICharacter> CharacterClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="AI Spawner")
	bool bIsSpawnOnStart;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="AI Spawner")
	bool bDoOnce = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="AI Spawner")
	AActor* SpawnTriggerActor;

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

private:
	bool bCanSpawn = true;

	void UnsubscribeFromTrigger();

	UPROPERTY()
	TScriptInterface<IInteractable> SpawnTrigger;
	FDelegateHandle TriggerHandle;
};
