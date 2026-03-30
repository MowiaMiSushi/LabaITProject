#include "BTTask_LabaITSetPatrolTarget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "LabaITPatrolComponent.h"
#include "AIController.h"
#include "GameFramework/Pawn.h"

UBTTask_LabaITSetPatrolTarget::UBTTask_LabaITSetPatrolTarget()
{
	NodeName = TEXT("LabaIT Set Patrol Target");
}

EBTNodeResult::Type UBTTask_LabaITSetPatrolTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp || BlackboardKey.SelectedKeyName.IsNone())
	{
		return EBTNodeResult::Failed;
	}

	AAIController* AIController = OwnerComp.GetAIOwner();
	APawn* Pawn = AIController ? AIController->GetPawn() : nullptr;
	if (!Pawn)
	{
		return EBTNodeResult::Failed;
	}

	const ULabaITPatrolComponent* Patrol = Pawn->FindComponentByClass<ULabaITPatrolComponent>();
	if (!Patrol || !Patrol->HasPatrolPoints())
	{
		return EBTNodeResult::Failed;
	}

	const FVector TargetLocation = Patrol->GetCurrentPatrolWorldLocation();
	BlackboardComp->SetValueAsVector(BlackboardKey.SelectedKeyName, TargetLocation);
	return EBTNodeResult::Succeeded;
}
