#pragma once

#include "CoreMinimal.h"
#include "GameCode/Actors/Interactive/InteractiveActor.h"
#include "GameCode/Actors/Interactive/Environment/Ladder.h"
#include "GameCode/Components/LedgeDetectorComponent.h"
#include "GameCode/Components/CharacterComponents/CharacterAttributeComponent.h"
#include "GameFramework/Character.h"
#include "GCBaseCharacter.generated.h"

class UGCBaseCharacterMovementComponent;

USTRUCT(BlueprintType)
struct FMantlingSettings
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimMontage* MantlingMontage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimMontage* FPMantlingMontage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UCurveVector* MantlingCurve;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(ClampMin=0, UIMin=0))
	float AnimationCorrectionXY = 65.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(ClampMin=0, UIMin=0))
	float AnimationCorrectionZ = 200.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(ClampMin=0, UIMin=0))
	float MinHeight = 1.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(ClampMin=0, UIMin=0))
	float MaxHeight = 200.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(ClampMin=0, UIMin=0))
	float MinHeightStartTime = 0.5f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(ClampMin=0, UIMin=0))
	float MaxHeightStartTime = 0.0f;
};


UCLASS(Abstract, NotBlueprintable)
class GAMECODE_API AGCBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AGCBaseCharacter(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	FORCEINLINE UGCBaseCharacterMovementComponent* GetBaseCharacterMovementComponent() const
	{
		return GCBaseCharacterMovementComponent;
	}

	void RegisterInteractiveActor(AInteractiveActor* InteractiveActor);
	void UnregisterInteractiveActor(AInteractiveActor* InteractiveActor);

	virtual void MoveForward(float Value)
	{
	}

	virtual void MoveRight(float Value)
	{
	}

	virtual void Turn(float Value)
	{
	}

	virtual void LookUp(float Value)
	{
	}

	virtual void TurnAtRate(float Value)
	{
	}

	virtual void LookUpAtRate(float Value)
	{
	}

	virtual void ChangeCrouchState();

	void StartSprint();
	void StopSprint();

	virtual void SwimForward(float Value)
	{
	}

	virtual void SwimRight(float Value)
	{
	}

	virtual void SwimUp(float Value)
	{
	}

	void Mantle(bool bForce = false);
	bool CanMantling() const;
	virtual void OnMantling(const FMantlingSettings& MantlingSettings, float MantlingAnimationStartTime);
	virtual bool CanJumpInternal_Implementation() const override;

	void ClimbLadderUp(float Value);
	void InteractWithLadder();

	const ALadder* GetAvailableLadder() const;
	
	virtual void Falling() override;
	virtual void NotifyJumpApex() override;
	virtual void Landed(const FHitResult& Hit) override;

protected:
	UFUNCTION(BlueprintImplementableEvent, Category="Character|Movement")
	void OnSprintStart();
	UFUNCTION(BlueprintImplementableEvent, Category="Character|Movement")
	void OnSprintEnd();

	virtual bool CanSprint();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character|Controls")
	float BaseTurnRate = 45.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character|Controls")
	float BaseLookUpRate = 45.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character|Movement")
	float SprintSpeed = 800.0f;


	UGCBaseCharacterMovementComponent* GCBaseCharacterMovementComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Character|Movement")
	ULedgeDetectorComponent* LedgeDetectorComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Character|Movement|Mantling")
	FMantlingSettings HeightMantlingSettings;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Character|Movement|Mantling")
	FMantlingSettings LowMantlingSettings;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Character|Movement|Mantling",
		meta=(ClampMin=0.0f, UIMin = 0.0f))
	float LowMantleMaxHeight = 125.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Character|Components")
	UCharacterAttributeComponent* CharacterAttributeComponent;

	virtual void OnDeath();
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Character|Animations")
	UAnimMontage* OnDeathAnimMontage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Character|Attributes")
	UCurveFloat* FallDamageCurve;
	

private:
	void TryChangeSprintState();
	const FMantlingSettings& GetMantlingSettings(float LedgeHeight) const;
	bool bIsSprintRequested = false;

	TArray<AInteractiveActor*> AvailableInteractiveActors;
	void EnableRagdoll();

	FVector CurrentFallApex;
};
