#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GCBaseCharacter.generated.h"

class UGCBaseCharacterMovementComponent;

UCLASS(Abstract, NotBlueprintable)
class GAMECODE_API AGCBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AGCBaseCharacter(const FObjectInitializer& ObjectInitializer);

	virtual void Tick(float DeltaSeconds) override;
	FORCEINLINE UGCBaseCharacterMovementComponent* GetBaseCharacterMovementComponent() {return GCBaseCharacterMovementComponent;}
	
	virtual void MoveForward(float Value){}
	virtual void MoveRight(float Value){}
	virtual void Turn(float Value){}
	virtual void LookUp(float Value){}
	
	virtual void TurnAtRate(float Value){}
	virtual void LookUpAtRate(float Value){}
	virtual void ChangeCrouchState();
	
	void StartSprint();
	void StopSprint();

	virtual void SwimForward(float Value) {}
	virtual void SwimRight(float Value) {}
	virtual void SwimUp(float Value) {}

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
	
private:
	void TryChangeSprintState();
	bool bIsSprintRequested = false;
};
