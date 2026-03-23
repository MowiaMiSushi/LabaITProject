// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LabaITPlayerController.generated.h"

class UHealthStaminaWidget;
class UUserWidget;
class APortal;

UCLASS()
class LABAITPROJECT_API ALabaITPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	/** Klasa widgetu z paskiem życia/staminy. Ustaw w Blueprintie (np. BPC_LabaIT) na WBP_HealthStaminaHUD. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HUD")
	TSubclassOf<UHealthStaminaWidget> HealthStaminaWidgetClass;

	/** Widget HUD po dodaniu do viewportu (do odczytu w Blueprint/C++). */
	UPROPERTY(BlueprintReadOnly, Category = "HUD")
	TObjectPtr<UHealthStaminaWidget> HealthStaminaWidget;

	/** Wywoływane, gdy gracz wejdzie w portal. W Blueprintie (np. BP_ShooterPlayerController) nadpisz to zdarzenie: pokaż UI „Przejść?”, a po zatwierdzeniu wywołaj Portal->ConfirmTravel(). */
	UFUNCTION(BlueprintImplementableEvent, Category = "Portal")
	void OnPortalTriggered(APortal* Portal);

	/** Pełny tryb gry: ukryty kursor, Game Only, fokus na viewport (np. po UI portalu lub po przejściu poziomu). */
	UFUNCTION(BlueprintCallable, Category = "Input")
	void RestoreGameOnlyInput();

	// --- Pauza (WBP_PauseMenu) ---

	/** Klasa menu pauzy (np. WBP_PauseMenu). Ustaw w Class Defaults Blueprintu kontrolera (kategoria Pause). */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Pause", meta = (DisplayName = "Pause Menu Widget Class"))
	TSubclassOf<UUserWidget> PauseMenuWidgetClass;

	/** Otwiera menu (pauza, kursor, UI) albo zamyka, jeśli już jest otwarte. Podłącz klawisz P w BP do tej funkcji. */
	UFUNCTION(BlueprintCallable, Category = "Pause")
	void TogglePauseMenu();

	/** Zamyka menu pauzy, odpauzowuje, przywraca grę. Przycisk „Wróć” w WBP: Get Owning Player → Close Pause Menu. */
	UFUNCTION(BlueprintCallable, Category = "Pause")
	void ClosePauseMenu();

protected:
	virtual void BeginPlay() override;

	/** Tworzy widget życia/staminy i dodaje go do ekranu. Wywoływane automatycznie w BeginPlay. */
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void ShowHealthStaminaHUD();

private:
	void TryApplyPendingSaveLoadFromGameInstance();

	void OpenPauseMenuInternal();

	UPROPERTY(Transient)
	TObjectPtr<UUserWidget> PauseMenuWidgetInstance;
};
