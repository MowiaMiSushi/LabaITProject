// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthStaminaWidget.generated.h"

class UHealthStaminaComponent;

UCLASS()
class LABAITPROJECT_API UHealthStaminaWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Health Stamina HUD")
	float GetHealthPercent() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Health Stamina HUD")
	float GetStaminaPercent() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Health Stamina HUD")
	FText GetHealthText() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Health Stamina HUD")
	FText GetStaminaText() const;

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UHealthStaminaComponent> HealthStaminaComponent;

	void FindPlayerHealthStamina();
};
