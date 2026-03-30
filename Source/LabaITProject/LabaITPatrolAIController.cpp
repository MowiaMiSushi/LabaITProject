#include "LabaITPatrolAIController.h"
#include "BehaviorTree/BehaviorTree.h"

ALabaITPatrolAIController::ALabaITPatrolAIController()
{
}

void ALabaITPatrolAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (PatrolBehaviorTree)
	{
		RunBehaviorTree(PatrolBehaviorTree);
	}
}
