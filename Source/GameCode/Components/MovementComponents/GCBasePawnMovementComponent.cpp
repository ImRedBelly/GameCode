#include "GCBasePawnMovementComponent.h"


void UGCBasePawnMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                 FActorComponentTickFunction* ThisTickFunction)
{
	if (ShouldSkipUpdate(DeltaTime))
	{
		return;
	}

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector PendingInput = GetPendingInputVector().GetClampedToMaxSize(1.0f);
	Velocity = PendingInput * MaxSpeed;
	ConsumeInputVector();

	if (bEnableGravity)
	{
		FHitResult HitResult;
		FVector StartPoint = UpdatedComponent->GetComponentLocation();
		float LineTraceLength = 50.0f + GetGravityZ() * DeltaTime;
		FVector EndPoint = StartPoint - LineTraceLength * FVector::UpVector;
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(GetOwner());

		bool bWasFalling = bIsFalling;

		bIsFalling = !GetWorld()->LineTraceSingleByChannel(HitResult, StartPoint, EndPoint, ECC_Visibility,
		                                                   CollisionParams);
		if (bIsFalling)
		{
			VerticalVelocity += GetGravityZ() * FVector::UpVector * DeltaTime;
			Velocity += VerticalVelocity;
		}
		else if (bWasFalling)
		{
			VerticalVelocity = FVector::ZeroVector;
		}
	}


	FVector Delta = Velocity * DeltaTime;

	if (!Delta.IsNearlyZero(1e-6f))
	{
		FQuat Rotation = UpdatedComponent->GetComponentQuat();
		FHitResult Hit(1.f);
		SafeMoveUpdatedComponent(Delta, Rotation, true, Hit);

		if (Hit.IsValidBlockingHit())
		{
			HandleImpact(Hit, DeltaTime, Delta);
			SlideAlongSurface(Delta, 1.f - Hit.Time, Hit.Normal, Hit, true);
		}
	}
	UpdateComponentVelocity();
}

void UGCBasePawnMovementComponent::JumpStart()
{
	VerticalVelocity = InitialJumpVelocity * FVector::UpVector;
}
