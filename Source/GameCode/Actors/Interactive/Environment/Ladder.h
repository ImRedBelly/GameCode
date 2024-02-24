#pragma once

#include "CoreMinimal.h"
#include "../InteractiveActor.h"
#include "Ladder.generated.h"

class UAnimMontage;
class UBoxComponent;
class UStaticMeshComponent;

UCLASS(Blueprintable)
class GAMECODE_API ALadder : public AInteractiveActor
{
	GENERATED_BODY()

public:
	ALadder();

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void BeginPlay() override;

	float GetLadderHeight() const;

	bool GetIsOnTop() const;
	UAnimMontage* GetAttachFromTopAnimMontage() const;
	FVector GetAttachFromTopAnimMontageStartingLocation() const;

protected:
	virtual void OnInteractionVolumeOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                                             UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                                             const FHitResult& SweepResult) override;
	virtual void OnInteractionVolumeOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                                           UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	UBoxComponent* GetLadderInteractionBox() const;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ladder parameters")
	float LadderHeight = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ladder parameters")
	float LadderWidth = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ladder parameters")
	float StepsInterval = 25.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ladder parameters")
	float BottomStepOffset = 25.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* RightRailMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* LeftRailMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UInstancedStaticMeshComponent* StepsMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBoxComponent* TopInteractionVolume;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ladder parameters")
	UAnimMontage* AttachFromTopAnimMontage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ladder parameters")
	FVector AttachFromTopAnimMontageInitialOffset = FVector::ZeroVector;

private:
	bool bIsOnTop = false;
};
