#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_LabaITSetPatrolTarget.generated.h"

/**
 * Zapisuje na Blackboardzie (Vector) pozycję świata bieżącego punktu patrolu z LabaITPatrolComponent.
 * Użyj przed wbudowanym Move To (cel z Blackboard).
 */
UCLASS()
class LABAITPROJECT_API UBTTask_LabaITSetPatrolTarget : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_LabaITSetPatrolTarget();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
