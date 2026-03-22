// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Portal.generated.h"

class UBoxComponent;
class ALabaITPlayerController;

UCLASS()
class LABAITPROJECT_API APortal : public AActor
{
	GENERATED_BODY()

public:
	APortal();

	virtual void BeginPlay() override;

	/** Wywołaj po zatwierdzeniu przez gracza (np. z UI). Ładuje poziom docelowy i ustawia miejsce odbioru. */
	UFUNCTION(BlueprintCallable, Category = "Portal")
	void ConfirmTravel();

	/** Wywołaj z innego aktora (np. BP_Gates w OnComponentBeginOverlap), gdy gracz wejdzie w Twoją strefę – wtedy pokaże się UI portalu. Zamiast TriggerBox możesz użyć własnego mesh/collision. */
	UFUNCTION(BlueprintCallable, Category = "Portal")
	void TriggerFromExternal();

	/** Wywołaj przy wyjściu gracza (np. BP_Gates w OnComponentEndOverlap), żeby portal mógł się znowu wywołać przy następnym wejściu. */
	UFUNCTION(BlueprintCallable, Category = "Portal")
	void TriggerFromExternalReset();

	/** Unikalna „nazwa” tego portalu – po wejściu w drugi portal gracz pojawi się przy portalu z tym tagiem w drugim poziomie. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Portal")
	FName PortalTag;

	/** Nazwa poziomu do załadowania (np. „Level2” albo pełna ścieżka). Puste = teleport w tym samym poziomie do portalu o TargetPortalTag. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Portal")
	FString TargetLevelName;

	/** Tag portalu w poziomie docelowym (albo w tym samym), przy którym postawić gracza. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Portal")
	FName TargetPortalTag;

protected:
	UPROPERTY(VisibleAnywhere, Category = "Portal")
	TObjectPtr<UBoxComponent> TriggerBox;

	UFUNCTION()
	void OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void TryTeleportPlayerToThisPortal();

	/** Ponownie ustawia tryb gry i kursor – wywoływane z opóźnieniem po załadowaniu poziomu. */
	void RestoreGameInputAfterPortalTravel();

private:
	bool bTriggeredThisOverlap = false;

	FTimerHandle RestoreInputTimerHandle;
	FTimerHandle RestoreInputTimerHandle2;
};
