#include "LabaITPatrolComponent.h"
#include "GameFramework/Actor.h"

ULabaITPatrolComponent::ULabaITPatrolComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void ULabaITPatrolComponent::BeginPlay()
{
	Super::BeginPlay();
	ClampIndexInRange();
}

FVector ULabaITPatrolComponent::GetCurrentPatrolWorldLocation() const
{
	if (!HasPatrolPoints())
	{
		const AActor* Owner = GetOwner();
		return Owner ? Owner->GetActorLocation() : FVector::ZeroVector;
	}

	const int32 Num = RelativePatrolPoints.Num();
	const int32 SafeIndex = FMath::Clamp(CurrentIndex, 0, Num - 1);
	return TransformPatrolPointToWorld(RelativePatrolPoints[SafeIndex]);
}

void ULabaITPatrolComponent::AdvanceToNextPatrolPoint()
{
	const int32 Num = RelativePatrolPoints.Num();
	if (Num <= 0)
	{
		return;
	}

	if (Num == 1)
	{
		CurrentIndex = 0;
		return;
	}

	if (PatrolMode == ELabaITPatrolMode::Cyclic)
	{
		CurrentIndex = (CurrentIndex + 1) % Num;
		return;
	}

	// PingPong: CurrentIndex = indeks punktu, do którego właśnie doszliśmy; ustawiamy następny.
	const int32 k = CurrentIndex;
	if (k == Num - 1)
	{
		PatrolDirection = -1;
	}
	else if (k == 0)
	{
		PatrolDirection = 1;
	}
	CurrentIndex = k + PatrolDirection;
	ClampIndexInRange();
}

void ULabaITPatrolComponent::SetPatrolMode(ELabaITPatrolMode NewMode)
{
	PatrolMode = NewMode;
	PatrolDirection = 1;
	ClampIndexInRange();
}

FVector ULabaITPatrolComponent::TransformPatrolPointToWorld(const FVector& LocalOffset) const
{
	const AActor* Owner = GetOwner();
	if (!Owner)
	{
		return LocalOffset;
	}

	return Owner->GetActorTransform().TransformPosition(LocalOffset);
}

void ULabaITPatrolComponent::ClampIndexInRange()
{
	const int32 Num = RelativePatrolPoints.Num();
	if (Num <= 0)
	{
		CurrentIndex = 0;
		return;
	}

	CurrentIndex = FMath::Clamp(CurrentIndex, 0, Num - 1);
}
