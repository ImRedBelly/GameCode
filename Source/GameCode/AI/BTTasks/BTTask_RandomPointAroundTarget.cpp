#include "BTTask_RandomPointAroundTarget.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameCode/AI/Controllers/AICharacterController.h"

UBTTask_RandomPointAroundTarget::UBTTask_RandomPointAroundTarget()
{
	NodeName = "Random point around target";
}

EBTNodeResult::Type UBTTask_RandomPointAroundTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();

	if (!IsValid(AIController) || !IsValid(Blackboard))
		return EBTNodeResult::Failed;

	APawn* Pawn = AIController->GetPawn();
	if (!IsValid(Pawn))
		return EBTNodeResult::Failed;

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(Pawn);
	if (!IsValid(NavSystem))
		return EBTNodeResult::Failed;

	AActor* TargetActor = Cast<AActor>(Blackboard->GetValueAsObject(TargetKey.SelectedKeyName));

	if (!IsValid(TargetActor))
		return EBTNodeResult::Failed;

	FNavLocation NavLocation;
	bool bIsFound = NavSystem->GetRandomReachablePointInRadius(TargetActor->GetActorLocation(), Radius, NavLocation);
	if (!bIsFound)
		return EBTNodeResult::Failed;
	
	Blackboard->SetValueAsVector(LocationKey.SelectedKeyName, NavLocation.Location);
	return EBTNodeResult::Succeeded;
}
