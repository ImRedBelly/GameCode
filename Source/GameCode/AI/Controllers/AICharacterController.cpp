#include "AICharacterController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AISense_Sight.h"


void AAICharacterController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
	if (IsValid(InPawn))
	{
		checkf(InPawn->IsA<AGCAICharacter>(), TEXT("AGCAICharacterController::SetPawn() AICharacterController can possess only GCAIACharacter"));
		CachedCharacter = StaticCast<AGCAICharacter*>(InPawn);
		RunBehaviorTree(CachedCharacter->GetBehaviorTree());
		SetupPatrolling();
	}
	else
	{
		CachedCharacter = nullptr;
	}
}

void AAICharacterController::ActorsPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	Super::ActorsPerceptionUpdated(UpdatedActors);
	if (!CachedCharacter.IsValid())
		return;

	TryMoveToNextTarget();
}

void AAICharacterController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);
	if (!Result.IsSuccess())
		return;

	TryMoveToNextTarget();
}

void AAICharacterController::SetupPatrolling()
{
	UAIPatrollingComponent* PatrollingComponent = CachedCharacter->GetPatrollingComponent();
	if (PatrollingComponent->CanPatrol())
	{
		FVector ClosestWayPoint = PatrollingComponent->SelectClosestWayPoint();
		if (IsValid(Blackboard))
		{
			Blackboard->SetValueAsVector(BB_NextLocation, ClosestWayPoint);
			Blackboard->SetValueAsObject(BB_CurrentTarget, nullptr);
		}
		bIsPatrolling = true;
	}
}

void AAICharacterController::TryMoveToNextTarget()
{
	AActor* ClosestActor = GetClosestSensedActor(UAISense_Sight::StaticClass());
	UAIPatrollingComponent* PatrollingComponent = CachedCharacter->GetPatrollingComponent();
	if (IsValid(ClosestActor))
	{
		if (IsValid(Blackboard))
		{
			Blackboard->SetValueAsObject(BB_CurrentTarget, ClosestActor);
			SetFocus(ClosestActor, EAIFocusPriority::Gameplay);
		}

		bIsPatrolling = false;
	}
	else if (PatrollingComponent->CanPatrol())
	{
		FVector WayPoint = bIsPatrolling ? PatrollingComponent->SelectNextWayPoint() : PatrollingComponent->SelectClosestWayPoint();
		if (IsValid(Blackboard))
		{
			ClearFocus(EAIFocusPriority::Gameplay);
			Blackboard->SetValueAsVector(BB_NextLocation, WayPoint);
			Blackboard->SetValueAsObject(BB_CurrentTarget, nullptr);
		}

		bIsPatrolling = true;
	}
}

bool AAICharacterController::IsTargetReached(FVector TargetLocation) const
{
	return (TargetLocation - CachedCharacter->GetActorLocation()).SizeSquared() <= FMath::Square(TargetReachedRadius);
}
