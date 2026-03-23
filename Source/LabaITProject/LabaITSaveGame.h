// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "LabaITSaveGame.generated.h"

/**
 * Dane zapisywane na dysk (USaveGame).
 * Unreal sam zapisuje wszystkie UPROPERTY do pliku slotu.
 */
UCLASS(BlueprintType)
class LABAITPROJECT_API ULabaITSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	/** Nazwa mapy (jak z GetCurrentLevelName: bez ścieżki, bez .umap). */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Save")
	FString SavedLevelName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Save")
	FVector PlayerLocation = FVector::ZeroVector;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Save")
	FRotator PlayerRotation = FRotator::ZeroRotator;

	/** Tekst do ekranu wyboru slotu (np. data). */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Save")
	FString SaveTimeDisplayString;
};
