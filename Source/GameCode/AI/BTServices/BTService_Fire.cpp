#include "BTService_Fire.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameCode/Actors/Equipment/Weapons/RangeWeaponItem.h"
#include "GameCode/AI/Characters/GCAICharacter.h"
#include "GameCode/Components/CharacterComponents/CharacterEquipmentComponent.h"

UBTService_Fire::UBTService_Fire()
{
	NodeName = "Fire";
}

void UBTService_Fire::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* AIController = OwnerComp.GetAIOwner();
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();

	if (!IsValid(AIController) || !IsValid(Blackboard))
		return;

	AGCBaseCharacter* Character = Cast<AGCBaseCharacter>(AIController->GetPawn());
	if (!IsValid(Character))
		return;

	const UCharacterEquipmentComponent* EquipmentComponent = Character->GetCharacterEquipmentComponent();
	ARangeWeaponItem* RangeWeapon = EquipmentComponent->GetCurrentRangeWeapon();
	if (!IsValid(RangeWeapon))
		return;

	AActor* CurrentTarget = Cast<AActor>(Blackboard->GetValueAsObject(TargetKey.SelectedKeyName));
	if (!IsValid(CurrentTarget))
	{
		Character->StopFire();
		return;
	}

	float DistanceSq = FVector::DistSquared(CurrentTarget->GetActorLocation(), Character->GetActorLocation());
	if (DistanceSq > FMath::Square(MaxFireDistance))
	{
		Character->StopFire();
		return;
	}

	if (!(RangeWeapon->IsReloading() || RangeWeapon->IsFiring()))
	{
		Character->StartFire();
	}
}
