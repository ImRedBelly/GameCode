#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "Components/BoxComponent.h"
#include "InteractiveCameraActor.generated.h"

UCLASS()
class GAMECODE_API AInteractiveCameraActor : public ACameraActor
{
	GENERATED_BODY()

public:
	AInteractiveCameraActor();

protected:
	virtual void BeginPlay() override;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBoxComponent* BoxComponent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Trasnition Settings")
	float TransitionToCameraTime = 2.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Trasnition Settings")
	float TransitionToPawnTime = 2.0f;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                  int32 OtherBodyIndex);
};
