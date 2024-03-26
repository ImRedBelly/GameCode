#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameCode/Actors/Interactive/Interface/IInteractable.h"
#include "GameFramework/Actor.h"
#include "Door.generated.h"

UCLASS()
class GAMECODE_API ADoor : public AActor, public IInteractable
{
	GENERATED_BODY()

public:
	ADoor();
	virtual void Tick(float DeltaTime) override;
	virtual void Interact(AGCBaseCharacter* Character) override;
	virtual FName GetActionEventName() const override;
	virtual bool HasOnInteractionCallback() const override;
	virtual FDelegateHandle AddOnInteractionUFunction(UObject* Object, const FName& FunctionName) override;
	virtual void RemoveOnInteractionUFunction(FDelegateHandle DelegateHandle) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Interactive | Door")
	UStaticMeshComponent* DoorMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Interactive | Door")
	USceneComponent* DoorPivot;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Interactive | Door")
	float AngleClosed = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Interactive | Door")
	float AngleOpen = 90;

	UPROPERTY(EditAnywhere	, BlueprintReadOnly, Category="Interactive | Door")
	UCurveFloat* DoorAnimationCurve;

	virtual void BeginPlay() override;

	FOnInteraction OnInteraction;

private:
	void InteractWithDoor();

	UFUNCTION()
	void UpdateDoorAnimation(float Alpha);

	UFUNCTION()
	void OnDoorAnimationFinished();

	FTimeline DoorOpenAnimTimeline;
	bool bIsOpnened = false;
};
