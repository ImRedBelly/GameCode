#include "AICharacterSpawner.h"

AAICharacterSpawner::AAICharacterSpawner()
{
	USceneComponent* SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SpawnerRoot"));
	SetRootComponent(SceneRoot);
}

void AAICharacterSpawner::SpawnAI()
{
	if (!bCanSpawn || !IsValid(CharacterClass))
		return;

	AGCAICharacter* AICharacter = GetWorld()->SpawnActor<AGCAICharacter>(CharacterClass, GetTransform());

	if (!IsValid(AICharacter->Controller))
		AICharacter->SpawnDefaultController();

	if (bDoOnce)
	{
		bCanSpawn = false;
		UnsubscribeFromTrigger();
	}
}

void AAICharacterSpawner::BeginPlay()
{
	Super::BeginPlay();

	if (SpawnTrigger.GetInterface())
		TriggerHandle = SpawnTrigger->AddOnInteractionUFunction(this, FName("SpawnAI"));

	if (bIsSpawnOnStart)
		SpawnAI();
}

void AAICharacterSpawner::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnsubscribeFromTrigger();
	Super::EndPlay(EndPlayReason);
}

void AAICharacterSpawner::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if (PropertyChangedEvent.Property->GetName() == GET_MEMBER_NAME_STRING_CHECKED(AAICharacterSpawner, SpawnTriggerActor))
	{
		SpawnTrigger = SpawnTriggerActor;
		if (SpawnTrigger.GetInterface())
		{
			if (!SpawnTrigger->HasOnInteractionCallback())
			{
				SpawnTriggerActor = nullptr;
				SpawnTrigger = nullptr;
			}
		}
		else
		{
			SpawnTriggerActor = nullptr;
			SpawnTrigger = nullptr;
		}
	}
}

void AAICharacterSpawner::UnsubscribeFromTrigger()
{
	if (TriggerHandle.IsValid() && SpawnTrigger.GetInterface())
		SpawnTrigger->RemoveOnInteractionUFunction(TriggerHandle);
}
