#pragma once

#include "CoreMinimal.h"
#include "PlayerCharacter.h"
#include "Controllers/GCPlayerController.h"
#include "FPPlayerCharacter.generated.h"

UCLASS()
class GAMECODE_API AFPPlayerCharacter : public APlayerCharacter
{
	GENERATED_BODY()

public:
	AFPPlayerCharacter(const FObjectInitializer& ObjectInitializer);

	virtual void PossessedBy(AController* NewController) override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;
	virtual void OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;
	virtual void OnMantling(const FMantlingSettings& MantlingSettings, float MantlingAnimationStartTime) override;
	virtual FRotator GetViewRotation() const override;
	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Character | First person")
	USkeletalMeshComponent* FirstPersonMeshComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Character | First person")
	UCameraComponent* FirstPersonCameraComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Character | First person| Camera | Ladder| Pitch",
		meta=(UIMin = -89, UIMax = 89))
	float LadderCameraMinPitch = -60;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Character | First person| Camera | Ladder| Pitch",
		meta=(UIMin = -89, UIMax = 89))
	float LadderCameraMaxPitch = 80;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Character | First person| Camera | Ladder| Yaw",
		meta=(UIMin = 0, UIMax = 359))
	float LadderCameraMinYaw = 5;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Character | First person| Camera | Ladder| Yaw",
		meta=(UIMin = 0, UIMax = 359))
	float LadderCameraMaxYaw = 175;

private:
	TWeakObjectPtr<AGCPlayerController> GCPlayerController;
	FTimerHandle FPMontageTimer;
	void OnFPMontageTimerElapsed();
	bool IsFPMontagePlaying() const;
void OnLadderStarted();
void OnLadderStopped();
};
