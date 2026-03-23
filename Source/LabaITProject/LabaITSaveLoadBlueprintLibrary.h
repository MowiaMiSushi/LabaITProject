// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "LabaITSaveLoadBlueprintLibrary.generated.h"

class ULabaITGameInstance;

/**
 * Krótkie wywołania z menu (Pauza / Main Menu) bez rzutowania w BP.
 * Prawy klik -> kontekst „Get Game Instance” też działa, ale tu jest jedna funkcja.
 */
UCLASS()
class LABAITPROJECT_API ULabaITSaveLoadBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "LabaIT|Save", meta = (WorldContext = "WorldContextObject"))
	static bool SaveCurrentGameToSlot(UObject* WorldContextObject, int32 SlotIndex);

	UFUNCTION(BlueprintCallable, Category = "LabaIT|Save", meta = (WorldContext = "WorldContextObject"))
	static bool LoadGameFromSlot(UObject* WorldContextObject, int32 SlotIndex);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "LabaIT|Save", meta = (WorldContext = "WorldContextObject"))
	static bool DoesSaveGameExistInSlot(UObject* WorldContextObject, int32 SlotIndex);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "LabaIT|Save", meta = (WorldContext = "WorldContextObject"))
	static int32 GetMaxSaveSlotCount(UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "LabaIT|Save", meta = (WorldContext = "WorldContextObject"))
	static FString GetSavedLevelNameForSlot(UObject* WorldContextObject, int32 SlotIndex);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "LabaIT|Save", meta = (WorldContext = "WorldContextObject"))
	static FString GetSavedTimeDisplayStringForSlot(UObject* WorldContextObject, int32 SlotIndex);

	UFUNCTION(BlueprintCallable, Category = "LabaIT|Save", meta = (WorldContext = "WorldContextObject"))
	static bool DeleteSaveGameInSlot(UObject* WorldContextObject, int32 SlotIndex);

	/** „Nowa gra”: OpenLevel(NewGameMapName) z Game Instance + reset flag zapisu/portalu. */
	UFUNCTION(BlueprintCallable, Category = "LabaIT|Menu", meta = (WorldContext = "WorldContextObject"))
	static bool StartNewGame(UObject* WorldContextObject);

private:
	static ULabaITGameInstance* GetLabaGameInstance(UObject* WorldContextObject);
};
