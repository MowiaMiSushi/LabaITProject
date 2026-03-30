#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "LabaITPatrolAIController.generated.h"

class UBehaviorTree;

/**
 * Uruchamia drzewo zachowań po opanowaniu pawna (OnPossess), zamiast z BeginPlay kontrolera —
 * wtedy GetPawn() i komponenty są już gotowe.
 */
UCLASS(Blueprintable)
class LABAITPROJECT_API ALabaITPatrolAIController : public AAIController
{
	GENERATED_BODY()

public:
	ALabaITPatrolAIController();

protected:
	virtual void OnPossess(APawn* InPawn) override;

	UPROPERTY(EditDefaultsOnly, Category = "AI|Patrol")
	TObjectPtr<UBehaviorTree> PatrolBehaviorTree;
};
