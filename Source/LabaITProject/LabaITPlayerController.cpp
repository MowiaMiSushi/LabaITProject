// Fill out your copyright notice in the Description page of Project Settings.

#include "LabaITPlayerController.h"
#include "LabaITGameInstance.h"
#include "HealthStaminaWidget.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/Pawn.h"
#include "Framework/Application/SlateApplication.h"
#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"
#include "TimerManager.h"

void ALabaITPlayerController::BeginPlay()
{
	Super::BeginPlay();
	ShowHealthStaminaHUD();

	TryApplyPendingSaveLoadFromGameInstance();

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

void ALabaITPlayerController::TogglePauseMenu()
{
	if (PauseMenuWidgetInstance && PauseMenuWidgetInstance->IsInViewport())
	{
		ClosePauseMenu();
	}
	else
	{
		OpenPauseMenuInternal();
	}
}

void ALabaITPlayerController::OpenPauseMenuInternal()
{
	if (!PauseMenuWidgetClass)
	{
		return;
	}
	if (PauseMenuWidgetInstance && PauseMenuWidgetInstance->IsInViewport())
	{
		return;
	}

	PauseMenuWidgetInstance = CreateWidget<UUserWidget>(this, PauseMenuWidgetClass);
	if (!PauseMenuWidgetInstance)
	{
		return;
	}

	PauseMenuWidgetInstance->AddToViewport(100);

	SetPause(true);
	SetShowMouseCursor(true);
	SetIgnoreMoveInput(true);
	SetIgnoreLookInput(true);

	FInputModeGameAndUI InputMode;
	InputMode.SetWidgetToFocus(PauseMenuWidgetInstance->TakeWidget());
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	SetInputMode(InputMode);
}

void ALabaITPlayerController::ClosePauseMenu()
{
	if (PauseMenuWidgetInstance)
	{
		PauseMenuWidgetInstance->RemoveFromParent();
		PauseMenuWidgetInstance = nullptr;
	}

	SetPause(false);
	RestoreGameOnlyInput();
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

void ALabaITPlayerController::TryApplyPendingSaveLoadFromGameInstance()
{
	ULabaITGameInstance* GI = Cast<ULabaITGameInstance>(GetGameInstance());
	if (!GI || !GI->HasPendingSaveLoadTransform())
	{
		return;
	}

	if (APawn* PlayerPawn = GetPawn())
	{
		GI->ApplyPendingSaveLoadIfAny(PlayerPawn);
		return;
	}

	if (UWorld* World = GetWorld())
	{
		FTimerHandle Handle;
		World->GetTimerManager().SetTimer(
			Handle,
			[this, GI]()
			{
				if (APawn* DeferredPawn = GetPawn())
				{
					GI->ApplyPendingSaveLoadIfAny(DeferredPawn);
				}
			},
			0.1f,
			false);
	}
}
