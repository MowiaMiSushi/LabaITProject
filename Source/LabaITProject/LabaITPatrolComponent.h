#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LabaITPatrolComponent.generated.h"

/** Tryb kolejnych punktów patrolu. */
UENUM(BlueprintType)
enum class ELabaITPatrolMode : uint8
{
	/** 0 -> 1 -> 2 -> 0 -> 1 ... */
	Cyclic UMETA(DisplayName = "Cyclic"),

	/** 0 -> 1 -> 2 -> 1 -> 0 -> 1 ... (odwracanie kierunku na końcach) */
	PingPong UMETA(DisplayName = "Ping Pong (reverse)")
};

/**
 * Punkty patrolu w przestrzeni lokalnej właściciela (względem aktora).
 * W edytorze ustawiasz offsety; w grze zamieniane na pozycje świata.
 */
UCLASS(ClassGroup = (AI), meta = (BlueprintSpawnableComponent))
class LABAITPROJECT_API ULabaITPatrolComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	ULabaITPatrolComponent();

	UFUNCTION(BlueprintPure, Category = "Patrol")
	bool HasPatrolPoints() const { return RelativePatrolPoints.Num() > 0; }

	UFUNCTION(BlueprintPure, Category = "Patrol")
	int32 GetNumPatrolPoints() const { return RelativePatrolPoints.Num(); }

	UFUNCTION(BlueprintPure, Category = "Patrol")
	int32 GetCurrentPatrolIndex() const { return CurrentIndex; }

	/** Pozycja świata dla aktualnego indeksu (bez przechodzenia dalej). */
	UFUNCTION(BlueprintPure, Category = "Patrol")
	FVector GetCurrentPatrolWorldLocation() const;

	/** Przechodzi do następnego punktu wg trybu (Cyclic / PingPong). */
	UFUNCTION(BlueprintCallable, Category = "Patrol")
	void AdvanceToNextPatrolPoint();

	UFUNCTION(BlueprintPure, Category = "Patrol")
	ELabaITPatrolMode GetPatrolMode() const { return PatrolMode; }

	UFUNCTION(BlueprintCallable, Category = "Patrol")
	void SetPatrolMode(ELabaITPatrolMode NewMode);

protected:
	virtual void BeginPlay() override;

	/** Offsety punktów względem aktora (lokalna przestrzeń). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Patrol", meta = (MakeEditWidget = "true"))
	TArray<FVector> RelativePatrolPoints;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Patrol")
	ELabaITPatrolMode PatrolMode = ELabaITPatrolMode::Cyclic;

private:
	UPROPERTY()
	int32 CurrentIndex = 0;

	/** +1 lub -1; używane w PingPong. */
	int8 PatrolDirection = 1;

	FVector TransformPatrolPointToWorld(const FVector& LocalOffset) const;
	void ClampIndexInRange();
};
