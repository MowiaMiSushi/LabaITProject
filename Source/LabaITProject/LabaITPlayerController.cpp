// Fill out your copyright notice in the Description page of Project Settings.

#include "LabaITPlayerController.h"
#include "HealthStaminaWidget.h"
#include "Blueprint/UserWidget.h"

void ALabaITPlayerController::BeginPlay()
{
	Super::BeginPlay();
	ShowHealthStaminaHUD();
}

void ALabaITPlayerController::ShowHealthStaminaHUD()
{
	if (!HealthStaminaWidgetClass) return;

	UHealthStaminaWidget* Widget = CreateWidget<UHealthStaminaWidget>(this, HealthStaminaWidgetClass);
	if (Widget)
	{
		Widget->AddToViewport();
		HealthStaminaWidget = Widget;
	}
}
