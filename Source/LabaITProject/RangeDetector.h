// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetMathLibrary.h"

#include "RangeDetector.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LABAITPROJECT_API URangeDetector : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URangeDetector();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	void CheckDistanceToTarget(float DeltaTime);
	void RotateToTarget(FVector thisLocation, FVector targetLocation, float DeltaTime);

	UPROPERTY(EditAnywhere, Category = "Detector")
	float DetectionRange = 500.0f;

	UPROPERTY(EditAnywhere, Category = "Detector")
	float RotationSpeed = 5.0f;
};
