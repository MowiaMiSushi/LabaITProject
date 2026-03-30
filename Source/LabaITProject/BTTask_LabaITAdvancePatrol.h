#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_LabaITAdvancePatrol.generated.h"

/**
 * Po udanym Move To wywołuje Advance na LabaITPatrolComponent (następny punkt wg Cyclic / PingPong).
 */
UCLASS()
class LABAITPROJECT_API UBTTask_LabaITAdvancePatrol : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_LabaITAdvancePatrol();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
