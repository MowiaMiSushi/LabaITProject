#include "BTTask_LabaITAdvancePatrol.h"
#include "LabaITPatrolComponent.h"
#include "AIController.h"
#include "GameFramework/Pawn.h"

UBTTask_LabaITAdvancePatrol::UBTTask_LabaITAdvancePatrol()
{
	NodeName = TEXT("LabaIT Advance Patrol");
}

EBTNodeResult::Type UBTTask_LabaITAdvancePatrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	APawn* Pawn = AIController ? AIController->GetPawn() : nullptr;
	if (!Pawn)
	{
		return EBTNodeResult::Failed;
	}

	ULabaITPatrolComponent* Patrol = Pawn->FindComponentByClass<ULabaITPatrolComponent>();
	if (!Patrol || !Patrol->HasPatrolPoints())
	{
		return EBTNodeResult::Failed;
	}

	Patrol->AdvanceToNextPatrolPoint();
	return EBTNodeResult::Succeeded;
}
