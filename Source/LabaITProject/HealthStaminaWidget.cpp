// Fill out your copyright notice in the Description page of Project Settings.

#include "HealthStaminaWidget.h"
#include "HealthStaminaComponent.h"
#include "GameFramework/Pawn.h"
#include "Kismet/GameplayStatics.h"

void UHealthStaminaWidget::NativeConstruct()
{
	Super::NativeConstruct();
	FindPlayerHealthStamina();
}

void UHealthStaminaWidget::FindPlayerHealthStamina()
{
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (!PlayerPawn) return;

	HealthStaminaComponent = PlayerPawn->FindComponentByClass<UHealthStaminaComponent>();
}

float UHealthStaminaWidget::GetHealthPercent() const
{
	return HealthStaminaComponent ? HealthStaminaComponent->GetHealthPercent() : 0.0f;
}

float UHealthStaminaWidget::GetStaminaPercent() const
{
	return HealthStaminaComponent ? HealthStaminaComponent->GetStaminaPercent() : 0.0f;
}

FText UHealthStaminaWidget::GetHealthText() const
{
	if (!HealthStaminaComponent) return FText::FromString(TEXT("0/0"));
	return FText::FromString(FString::Printf(TEXT("%.0f / %.0f"),
		HealthStaminaComponent->GetCurrentHealth(),
		HealthStaminaComponent->GetMaxHealth()));
}

FText UHealthStaminaWidget::GetStaminaText() const
{
	if (!HealthStaminaComponent) return FText::FromString(TEXT("0/0"));
	return FText::FromString(FString::Printf(TEXT("%.0f / %.0f"),
		HealthStaminaComponent->GetCurrentStamina(),
		HealthStaminaComponent->GetMaxStamina()));
}
