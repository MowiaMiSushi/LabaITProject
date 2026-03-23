// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LabaITMainMenuWidget.generated.h"

/**
 * Bazowy widżet pod WBP_MainMenu (menu główne).
 * Rodzic: User Widget — możesz tu trzymać wspólne zmienne (np. wybrany slot zapisu).
 * Logika Save/Load jest w ULabaITGameInstance / ULabaITSaveLoadBlueprintLibrary (wywołania z Event Graph).
 */
UCLASS()
class LABAITPROJECT_API ULabaITMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/** Wybrany slot (0 .. Get Max Save Slot Count − 1). Ustaw z przycisków / listy w Blueprint. */
	UPROPERTY(BlueprintReadWrite, Category = "LabaIT|Save")
	int32 SelectedSaveSlotIndex = 0;
};
