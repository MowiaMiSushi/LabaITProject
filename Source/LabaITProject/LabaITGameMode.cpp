// Fill out your copyright notice in the Description page of Project Settings.

#include "LabaITGameMode.h"
#include "LabaITGameInstance.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Pawn.h"

void ALabaITGameMode::RestartPlayer(AController* NewPlayer)
{
	Super::RestartPlayer(NewPlayer);
	if (!NewPlayer)
	{
		return;
	}
	if (APawn* Pawn = NewPlayer->GetPawn())
	{
		if (ULabaITGameInstance* GI = Cast<ULabaITGameInstance>(GetGameInstance()))
		{
			GI->ApplyPendingSaveLoadIfAny(Pawn);
		}
	}
}
