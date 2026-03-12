
#include "RangeDetector.h"


URangeDetector::URangeDetector()
{
	
	PrimaryComponentTick.bCanEverTick = true;

	
}



void URangeDetector::BeginPlay()
{
	Super::BeginPlay();

	
	
}



void URangeDetector::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CheckDistanceToTarget(DeltaTime);
}


void URangeDetector::CheckDistanceToTarget(float DeltaTime)
{
	float DistanceToTarget = 0.0f;
	FVector targetLocation;
	FVector thisLocation = GetOwner()->GetActorLocation();							

	APawn* PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	
	if (PlayerPawn == nullptr) return;

	targetLocation = PlayerPawn->GetActorLocation();
	DistanceToTarget = FVector::Distance(thisLocation, targetLocation);			

	if (DistanceToTarget > DetectionRange) return;

	FVector DirectionToPlayer = (targetLocation - thisLocation).GetSafeNormal(); 
	FVector ForwardVector = GetOwner()->GetActorForwardVector();

	if (FVector::DotProduct(ForwardVector, DirectionToPlayer) <= 0.0f) return; 

	FHitResult Result;
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(GetOwner()); 
			
	bool bHit = GetWorld()->LineTraceSingleByChannel(Result, thisLocation, targetLocation, ECC_Pawn, TraceParams);

	if (bHit && Result.GetActor() == PlayerPawn)
	{
		RotateToTarget(thisLocation, targetLocation, DeltaTime);
	}
}


void URangeDetector::RotateToTarget(FVector thisLocation, FVector targetLocation, float DeltaTime)
{
	FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(thisLocation, targetLocation);
	FRotator CurrentRotation = GetOwner()->GetActorRotation();

	FRotator YLookRotation = FRotator(CurrentRotation.Pitch, TargetRotation.Yaw, CurrentRotation.Roll);

	FRotator SmoothRotation = FMath::RInterpTo(CurrentRotation, YLookRotation, DeltaTime, RotationSpeed); 
	GetOwner()->SetActorRotation(SmoothRotation);
}
