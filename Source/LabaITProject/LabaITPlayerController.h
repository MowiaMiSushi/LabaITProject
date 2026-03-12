// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LabaITPlayerController.generated.h"

class UHealthStaminaWidget;
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

protected:
	virtual void BeginPlay() override;

	/** Tworzy widget życia/staminy i dodaje go do ekranu. Wywoływane automatycznie w BeginPlay. */
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void ShowHealthStaminaHUD();
};
