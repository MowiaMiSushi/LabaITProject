// Fill out your copyright notice in the Description page of Project Settings.

#include "LabaITSaveLoadBlueprintLibrary.h"
#include "LabaITGameInstance.h"
#include "Engine/Engine.h"
#include "Engine/World.h"

ULabaITGameInstance* ULabaITSaveLoadBlueprintLibrary::GetLabaGameInstance(UObject* WorldContextObject)
{
	if (!WorldContextObject || !GEngine)
	{
		return nullptr;
	}
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull);
	return World ? Cast<ULabaITGameInstance>(World->GetGameInstance()) : nullptr;
}

bool ULabaITSaveLoadBlueprintLibrary::SaveCurrentGameToSlot(UObject* WorldContextObject, int32 SlotIndex)
{
	ULabaITGameInstance* GI = GetLabaGameInstance(WorldContextObject);
	return GI ? GI->SaveCurrentGameToSlot(SlotIndex) : false;
}

bool ULabaITSaveLoadBlueprintLibrary::LoadGameFromSlot(UObject* WorldContextObject, int32 SlotIndex)
{
	ULabaITGameInstance* GI = GetLabaGameInstance(WorldContextObject);
	return GI ? GI->LoadGameFromSlot(SlotIndex) : false;
}

bool ULabaITSaveLoadBlueprintLibrary::DoesSaveGameExistInSlot(UObject* WorldContextObject, int32 SlotIndex)
{
	ULabaITGameInstance* GI = GetLabaGameInstance(WorldContextObject);
	return GI ? GI->DoesSaveGameExistInSlot(SlotIndex) : false;
}

int32 ULabaITSaveLoadBlueprintLibrary::GetMaxSaveSlotCount(UObject* WorldContextObject)
{
	ULabaITGameInstance* GI = GetLabaGameInstance(WorldContextObject);
	return GI ? GI->MaxSaveSlots : 0;
}

FString ULabaITSaveLoadBlueprintLibrary::GetSavedLevelNameForSlot(UObject* WorldContextObject, int32 SlotIndex)
{
	ULabaITGameInstance* GI = GetLabaGameInstance(WorldContextObject);
	return GI ? GI->GetSavedLevelNameForSlot(SlotIndex) : FString();
}

FString ULabaITSaveLoadBlueprintLibrary::GetSavedTimeDisplayStringForSlot(UObject* WorldContextObject, int32 SlotIndex)
{
	ULabaITGameInstance* GI = GetLabaGameInstance(WorldContextObject);
	return GI ? GI->GetSavedTimeDisplayStringForSlot(SlotIndex) : FString();
}

bool ULabaITSaveLoadBlueprintLibrary::DeleteSaveGameInSlot(UObject* WorldContextObject, int32 SlotIndex)
{
	ULabaITGameInstance* GI = GetLabaGameInstance(WorldContextObject);
	return GI ? GI->DeleteSaveGameInSlot(SlotIndex) : false;
}

bool ULabaITSaveLoadBlueprintLibrary::StartNewGame(UObject* WorldContextObject)
{
	ULabaITGameInstance* GI = GetLabaGameInstance(WorldContextObject);
	return GI ? GI->StartNewGame() : false;
}
