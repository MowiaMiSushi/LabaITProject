// Fill out your copyright notice in the Description page of Project Settings.

#include "LabaITGameInstance.h"
#include "LabaITSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "Misc/DateTime.h"

bool ULabaITGameInstance::IsSlotIndexValid(int32 SlotIndex) const
{
	return SlotIndex >= 0 && SlotIndex < MaxSaveSlots;
}

FString ULabaITGameInstance::GetSlotNameForIndex(int32 SlotIndex) const
{
	return FString::Printf(TEXT("LabaITSave_%d"), SlotIndex);
}

bool ULabaITGameInstance::SaveCurrentGameToSlot(int32 SlotIndex)
{
	if (!IsSlotIndexValid(SlotIndex))
	{
		return false;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return false;
	}

	APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0);
	APawn* Pawn = PC ? PC->GetPawn() : nullptr;
	if (!Pawn)
	{
		return false;
	}

	ULabaITSaveGame* SaveObject = Cast<ULabaITSaveGame>(UGameplayStatics::CreateSaveGameObject(ULabaITSaveGame::StaticClass()));
	if (!SaveObject)
	{
		return false;
	}

	SaveObject->SavedLevelName = UGameplayStatics::GetCurrentLevelName(World, true);
	SaveObject->PlayerLocation = Pawn->GetActorLocation();
	SaveObject->PlayerRotation = Pawn->GetActorRotation();
	SaveObject->SaveTimeDisplayString = FDateTime::Now().ToString(TEXT("%Y-%m-%d %H:%M"));

	return UGameplayStatics::SaveGameToSlot(SaveObject, GetSlotNameForIndex(SlotIndex), SaveUserIndex);
}

bool ULabaITGameInstance::LoadGameFromSlot(int32 SlotIndex)
{
	if (!IsSlotIndexValid(SlotIndex))
	{
		return false;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return false;
	}

	ULabaITSaveGame* Loaded = Cast<ULabaITSaveGame>(
		UGameplayStatics::LoadGameFromSlot(GetSlotNameForIndex(SlotIndex), SaveUserIndex));
	if (!Loaded || Loaded->SavedLevelName.IsEmpty())
	{
		return false;
	}

	// Żeby portal w BeginPlay nie nadpisał pozycji zapisem „teleport przy portalu”
	PendingTeleportPortalTag = NAME_None;

	bPendingSaveLoadTransform = true;
	PendingSaveLoadLocation = Loaded->PlayerLocation;
	PendingSaveLoadRotation = Loaded->PlayerRotation;

	UGameplayStatics::OpenLevel(World, FName(*Loaded->SavedLevelName));
	return true;
}

bool ULabaITGameInstance::DoesSaveGameExistInSlot(int32 SlotIndex) const
{
	if (!IsSlotIndexValid(SlotIndex))
	{
		return false;
	}
	return UGameplayStatics::DoesSaveGameExist(GetSlotNameForIndex(SlotIndex), SaveUserIndex);
}

FString ULabaITGameInstance::GetSavedLevelNameForSlot(int32 SlotIndex) const
{
	if (!IsSlotIndexValid(SlotIndex) || !DoesSaveGameExistInSlot(SlotIndex))
	{
		return FString();
	}

	ULabaITSaveGame* Loaded = Cast<ULabaITSaveGame>(
		UGameplayStatics::LoadGameFromSlot(GetSlotNameForIndex(SlotIndex), SaveUserIndex));
	return Loaded ? Loaded->SavedLevelName : FString();
}

FString ULabaITGameInstance::GetSavedTimeDisplayStringForSlot(int32 SlotIndex) const
{
	if (!IsSlotIndexValid(SlotIndex) || !DoesSaveGameExistInSlot(SlotIndex))
	{
		return FString();
	}

	ULabaITSaveGame* Loaded = Cast<ULabaITSaveGame>(
		UGameplayStatics::LoadGameFromSlot(GetSlotNameForIndex(SlotIndex), SaveUserIndex));
	return Loaded ? Loaded->SaveTimeDisplayString : FString();
}

bool ULabaITGameInstance::DeleteSaveGameInSlot(int32 SlotIndex)
{
	if (!IsSlotIndexValid(SlotIndex))
	{
		return false;
	}
	return UGameplayStatics::DeleteGameInSlot(GetSlotNameForIndex(SlotIndex), SaveUserIndex);
}

void ULabaITGameInstance::ApplyPendingSaveLoadIfAny(APawn* PlayerPawn)
{
	if (!bPendingSaveLoadTransform || !PlayerPawn)
	{
		return;
	}

	PlayerPawn->SetActorLocationAndRotation(
		PendingSaveLoadLocation,
		PendingSaveLoadRotation,
		false,
		nullptr,
		ETeleportType::TeleportPhysics);

	bPendingSaveLoadTransform = false;
}

bool ULabaITGameInstance::StartNewGame()
{
	PendingTeleportPortalTag = NAME_None;
	bPendingSaveLoadTransform = false;
	PendingSaveLoadLocation = FVector::ZeroVector;
	PendingSaveLoadRotation = FRotator::ZeroRotator;

	UWorld* World = GetWorld();
	if (!World || NewGameMapName.IsNone())
	{
		return false;
	}

	UGameplayStatics::OpenLevel(World, NewGameMapName);
	return true;
}
