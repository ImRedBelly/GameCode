#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/Pawn.h"
#include "Components/SphereComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameCodeBasePawn.generated.h"

UCLASS()
class GAMECODE_API AGameCodeBasePawn : public APawn
{
	GENERATED_BODY()

public:
	AGameCodeBasePawn();
	UPROPERTY(VisibleAnywhere)
	UPawnMovementComponent* MovementComponent;
	UPROPERTY(VisibleAnywhere)
	USphereComponent* CollisionComponent;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void MoveForward(float Value);
	void MoveRight(float Value);
	void Jump();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetInputForward() { return InputForward; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetInputRight() { return InputRight; }

protected:
	virtual void BeginPlay() override;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Base pawn")
	float CollisionSphereRadius = 50.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Base pawn")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Base pawn")
	UCameraComponent* CameraComponent;

#if WITH_EDITORONLY_DATA
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Base pawn")
	UArrowComponent* ArrowComponent;
#endif

private:
	AActor* CurrentViewActor;

	UFUNCTION()
	void OnBlendComplete();

	float InputForward = 0.0f;
	float InputRight = 0.0f;
};
