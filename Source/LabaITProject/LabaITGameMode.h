// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LabaITGameMode.generated.h"

/**
 * Opcjonalny Game Mode: po odrodzeniu gracza stosuje pozycję z wczytanego zapisu,
 * jeśli PlayerController nie jest klasą LabaIT (zapasowy tor).
 * Ustaw w World Settings -> GameMode Override (lub dziedzicz w BP).
 */
UCLASS()
class LABAITPROJECT_API ALabaITGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void RestartPlayer(AController* NewPlayer) override;
};
