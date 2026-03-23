// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "LabaITGameInstance.generated.h"

class APawn;
class ULabaITSaveGame;

/**
 * Game Instance: portale + zapis/odczyt gry (sloty).
 * Ustaw w Project Settings -> Maps & Modes -> Game Instance Class.
 */
UCLASS()
class LABAITPROJECT_API ULabaITGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	/** Tag portalu w załadowanym poziomie – przy nim ustawiamy gracza. Ustawiane przed OpenLevel. */
	UPROPERTY(BlueprintReadWrite, Category = "Portal")
	FName PendingTeleportPortalTag;

	// --- Save / Load ---

	/** Ile slotów (0 .. MaxSaveSlots-1). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Save|Config", meta = (ClampMin = "1", ClampMax = "20"))
	int32 MaxSaveSlots = 5;

	/** Domyślny indeks użytkownika dla USaveGame (single player = 0). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Save|Config")
	int32 SaveUserIndex = 0;

	/** Zapisuje bieżącą mapę i pozycję/obrót piona gracza 0. */
	UFUNCTION(BlueprintCallable, Category = "Save")
	bool SaveCurrentGameToSlot(int32 SlotIndex);

	/** Wczytuje slot: ładuje mapę i po wejściu ustawia pozycję gracza (patrz ApplyPendingSaveLoadIfAny). */
	UFUNCTION(BlueprintCallable, Category = "Save")
	bool LoadGameFromSlot(int32 SlotIndex);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Save")
	bool DoesSaveGameExistInSlot(int32 SlotIndex) const;

	/** Nazwa zapisanej mapy do podglądu w menu (pusty string jeśli brak zapisu). */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Save")
	FString GetSavedLevelNameForSlot(int32 SlotIndex) const;

	/** Tekst czasu zapisu do UI. */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Save")
	FString GetSavedTimeDisplayStringForSlot(int32 SlotIndex) const;

	UFUNCTION(BlueprintCallable, Category = "Save")
	bool DeleteSaveGameInSlot(int32 SlotIndex);

	/** Czy po Load czekamy na postawienie piona (wywoływane z PlayerController). */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Save")
	bool HasPendingSaveLoadTransform() const { return bPendingSaveLoadTransform; }

	/** Stosuje zapisaną pozycję do piona i czyści flagę. Wywołuje C++ z PlayerController. */
	void ApplyPendingSaveLoadIfAny(APawn* PlayerPawn);

	// --- Menu ---

	/** Mapa startowa dla „Nowa gra” (np. Lvl_FirstPerson). Ustaw w domyślach Game Instance (BP) albo w szczegółach klasy. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Menu")
	FName NewGameMapName;

	/** Ładuje New Game Map Name; czyści oczekiwanie na teleport z portalu / pozycję z wczytanego save. */
	UFUNCTION(BlueprintCallable, Category = "Menu")
	bool StartNewGame();

protected:
	bool IsSlotIndexValid(int32 SlotIndex) const;
	FString GetSlotNameForIndex(int32 SlotIndex) const;

	/** Po Load: teleport na zapisane współrzędne (po załadowaniu poziomu). */
	bool bPendingSaveLoadTransform = false;
	FVector PendingSaveLoadLocation = FVector::ZeroVector;
	FRotator PendingSaveLoadRotation = FRotator::ZeroRotator;
};
