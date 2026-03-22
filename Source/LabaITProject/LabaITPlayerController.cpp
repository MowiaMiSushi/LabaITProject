// Fill out your copyright notice in the Description page of Project Settings.

#include "LabaITPlayerController.h"
#include "HealthStaminaWidget.h"
#include "Blueprint/UserWidget.h"
#include "Framework/Application/SlateApplication.h"
#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"
#include "TimerManager.h"

void ALabaITPlayerController::BeginPlay()
{
	Super::BeginPlay();
	ShowHealthStaminaHUD();

	// HUD (AddToViewport) może przejąć fokus po travelu – przywróć input po HUD + z krótkim opóźnieniem
	RestoreGameOnlyInput();
	if (UWorld* World = GetWorld())
	{
		FTimerHandle H;
		World->GetTimerManager().SetTimer(H, FTimerDelegate::CreateUObject(this, &ALabaITPlayerController::RestoreGameOnlyInput), 0.12f, false);
	}
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

void ALabaITPlayerController::RestoreGameOnlyInput()
{
	SetShowMouseCursor(false);
	SetIgnoreMoveInput(false);
	SetIgnoreLookInput(false);

	FInputModeGameOnly InputMode;
	InputMode.SetConsumeCaptureMouseDown(true);
	SetInputMode(InputMode);
	FlushPressedKeys();

	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().SetAllUserFocusToGameViewport();
	}

	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->SetMouseLockMode(EMouseLockMode::LockOnCapture);
		GEngine->GameViewport->SetMouseCaptureMode(EMouseCaptureMode::CapturePermanently_IncludingInitialMouseDown);
	}
}
