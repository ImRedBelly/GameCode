#include "AIPatrollingComponent.h"

bool UAIPatrollingComponent::CanPatrol() const
{
	return IsValid(PatrollingPath) && PatrollingPath->GetWayPoints().Num() > 0;
}

FVector UAIPatrollingComponent::SelectClosestWayPoint()
{
	FVector OwnerLocation = GetOwner()->GetActorLocation();
	const TArray<FVector> WayPoints = PatrollingPath->GetWayPoints();
	FTransform PathTransform = PatrollingPath->GetActorTransform();

	FVector ClosestWayPoint;
	float MinSqDistance = FLT_MAX;

	for (int32 i = 0; i < WayPoints.Num(); ++i)
	{
		FVector WayPointWorld = PathTransform.TransformPosition(WayPoints[i]);
		float CurrentDistance = (OwnerLocation - WayPointWorld).SizeSquared();
		if (CurrentDistance < MinSqDistance)
		{
			ClosestWayPoint = WayPointWorld;
			MinSqDistance = CurrentDistance;
			CurrentWayPointIndex = i;
		}
	}

	return ClosestWayPoint;
}

FVector UAIPatrollingComponent::SelectNextWayPoint()
{
	++CurrentWayPointIndex;
	const TArray<FVector> WayPoints = PatrollingPath->GetWayPoints();
	if (CurrentWayPointIndex >= WayPoints.Num())
		CurrentWayPointIndex = 0;

	FTransform PathTransform = PatrollingPath->GetActorTransform();
	FVector NextWayPoint = PathTransform.TransformPosition(WayPoints[CurrentWayPointIndex]);
	return NextWayPoint;
}
