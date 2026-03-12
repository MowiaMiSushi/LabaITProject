// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "LabaITGameInstance.generated.h"

/**
 * Przechowuje informację, przy którym portalu w nowym poziomie ustawić gracza.
 * Ustaw tę klasę w Project Settings -> Maps & Modes -> Game Instance Class.
 */
UCLASS()
class LABAITPROJECT_API ULabaITGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	/** Tag portalu w załadowanym poziomie – przy nim ustawiamy gracza. Ustawiane przed OpenLevel. */
	UPROPERTY(BlueprintReadWrite, Category = "Portal")
	FName PendingTeleportPortalTag;
};
