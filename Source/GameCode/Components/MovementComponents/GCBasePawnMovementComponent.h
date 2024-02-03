#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GCBasePawnMovementComponent.generated.h"


UCLASS()
class GAMECODE_API UGCBasePawnMovementComponent : public UPawnMovementComponent
{
	GENERATED_BODY()

public:
	void JumpStart();
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	UPROPERTY(EditAnywhere)
	float MaxSpeed = 1200.0f;
	UPROPERTY(EditAnywhere)
	float InitialJumpVelocity = 500.0f;
	UPROPERTY(EditAnywhere)
	bool bEnableGravity;

private:
	FVector VerticalVelocity = FVector::ZeroVector;
	bool bIsFalling = false;
};
