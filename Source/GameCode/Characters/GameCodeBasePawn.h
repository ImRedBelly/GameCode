#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h" 
#include "Components/SphereComponent.h"
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
};
