#include "GCAICharacterController.h"
#include "Perception/AISense_Sight.h"
#include "GameCode/AI/Characters/GCAICharacter.h"

void AGCAICharacterController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
	if (IsValid(InPawn))
	{
		checkf(InPawn->IsA<AGCAICharacter>(), TEXT("AGCAICharacterController::SetPawn() AICharacterController can possess only GCAIACharacter"));
		CachedCharacter = StaticCast<AGCAICharacter*>(InPawn);
	}
	else
	{
		CachedCharacter = nullptr;
	}
}

void AGCAICharacterController::ActorsPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	Super::ActorsPerceptionUpdated(UpdatedActors);
	if (!CachedCharacter.IsValid())
		return;

	TryMoveToNextTarget();
}

void AGCAICharacterController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);
	if (!Result.IsSuccess())
		return;

	TryMoveToNextTarget();
}

void AGCAICharacterController::BeginPlay()
{
	Super::BeginPlay();
	UAIPatrollingComponent* PatrollingComponent = CachedCharacter->GetPatrollingComponent();
	if (PatrollingComponent->CanPatrol())
	{
		FVector ClosestWayPoint = PatrollingComponent->SelectClosestWayPoint();
		MoveToLocation(ClosestWayPoint);
		bIsPatrolling = true;
	}
}

void AGCAICharacterController::TryMoveToNextTarget()
{
	AActor* ClosestActor = GetClosestSensedActor(UAISense_Sight::StaticClass());
	UAIPatrollingComponent* PatrollingComponent = CachedCharacter->GetPatrollingComponent();
	if (IsValid(ClosestActor))
	{
		if (!IsTargetReached(ClosestActor->GetActorLocation()))
			MoveToActor(ClosestActor);

		bIsPatrolling = false;
	}
	else if (PatrollingComponent->CanPatrol())
	{
		FVector WayPoint = bIsPatrolling ? PatrollingComponent->SelectNextWayPoint() : PatrollingComponent->SelectClosestWayPoint();
		if (!IsTargetReached(WayPoint))
			MoveToLocation(WayPoint);

		bIsPatrolling = true;
	}
}

bool AGCAICharacterController::IsTargetReached(FVector TargetLocation) const
{
	return (TargetLocation - CachedCharacter->GetActorLocation()).SizeSquared() <= FMath::Square(TargetReachedRadius);
}
