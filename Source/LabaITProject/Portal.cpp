// Fill out your copyright notice in the Description page of Project Settings.

#include "Portal.h"
#include "LabaITGameInstance.h"
#include "LabaITPlayerController.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerStart.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

APortal::APortal()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->SetupAttachment(RootComponent);
	TriggerBox->SetBoxExtent(FVector(80.0f, 80.0f, 120.0f));
	TriggerBox->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
}

void APortal::BeginPlay()
{
	Super::BeginPlay();

	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &APortal::OnTriggerBeginOverlap);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &APortal::OnTriggerEndOverlap);

	if (PortalTag != NAME_None)
	{
		Tags.AddUnique(PortalTag);
	}

	TryTeleportPlayerToThisPortal();
}

void APortal::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APawn* PlayerPawn = GetWorld()->GetFirstPlayerController() ? GetWorld()->GetFirstPlayerController()->GetPawn() : nullptr;
	if (!PlayerPawn || OtherActor != PlayerPawn || bTriggeredThisOverlap) return;

	ALabaITPlayerController* PC = Cast<ALabaITPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PC)
	{
		bTriggeredThisOverlap = true;
		PC->OnPortalTriggered(this);
	}
}

void APortal::OnTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	APawn* PlayerPawn = GetWorld()->GetFirstPlayerController() ? GetWorld()->GetFirstPlayerController()->GetPawn() : nullptr;
	if (OtherActor == PlayerPawn)
	{
		bTriggeredThisOverlap = false;
	}
}

void APortal::TriggerFromExternal()
{
	APawn* PlayerPawn = GetWorld()->GetFirstPlayerController() ? GetWorld()->GetFirstPlayerController()->GetPawn() : nullptr;
	if (!PlayerPawn || bTriggeredThisOverlap) return;

	ALabaITPlayerController* PC = Cast<ALabaITPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PC)
	{
		bTriggeredThisOverlap = true;
		PC->OnPortalTriggered(this);
	}
}

void APortal::TriggerFromExternalReset()
{
	bTriggeredThisOverlap = false;
}

void APortal::ConfirmTravel()
{
	if (TargetLevelName.IsEmpty())
	{
		TArray<AActor*> Found;
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), TargetPortalTag, Found);
		if (Found.Num() > 0 && GetWorld()->GetFirstPlayerController() && GetWorld()->GetFirstPlayerController()->GetPawn())
		{
			FVector Loc = Found[0]->GetActorLocation();
			FRotator Rot = Found[0]->GetActorRotation();
			GetWorld()->GetFirstPlayerController()->GetPawn()->SetActorLocationAndRotation(Loc, Rot);
		}
		RestoreGameInputAfterPortalTravel();
		return;
	}

	// Ważne: OpenLevel przerywa łańcuch Blueprintu – „RemoveFromParent” po ConfirmTravel często się nie wykona.
	// Przed podróżą zdejmij stan UI (kursor + tryb), żeby nowy poziom nie startował „jak pod UI”.
	if (ALabaITPlayerController* LabaPC = Cast<ALabaITPlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		LabaPC->RestoreGameOnlyInput();
	}
	else if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		PC->SetShowMouseCursor(false);
		FInputModeGameOnly InputMode;
		InputMode.SetConsumeCaptureMouseDown(true);
		PC->SetInputMode(InputMode);
		PC->FlushPressedKeys();
	}

	ULabaITGameInstance* GI = Cast<ULabaITGameInstance>(GetGameInstance());
	if (GI)
	{
		GI->PendingTeleportPortalTag = TargetPortalTag;
	}
	UGameplayStatics::OpenLevel(this, FName(*TargetLevelName));
}

void APortal::TryTeleportPlayerToThisPortal()
{
	ULabaITGameInstance* GI = Cast<ULabaITGameInstance>(GetGameInstance());
	if (!GI || GI->PendingTeleportPortalTag == NAME_None) return;
	if (PortalTag == NAME_None || GI->PendingTeleportPortalTag != PortalTag) return;

	APawn* PlayerPawn = GetWorld()->GetFirstPlayerController() ? GetWorld()->GetFirstPlayerController()->GetPawn() : nullptr;
	if (!PlayerPawn)
	{
		GI->PendingTeleportPortalTag = NAME_None;
		return;
	}

	// Spawn przy Player Start na nowej mapie (żeby nie stać w triggerze portalu i nie wywołać UI od razu)
	TArray<AActor*> PlayerStarts;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), PlayerStarts);
	if (PlayerStarts.Num() > 0)
	{
		AActor* Start = PlayerStarts[0];
		PlayerPawn->SetActorLocationAndRotation(Start->GetActorLocation(), Start->GetActorRotation());
	}
	else
	{
		// Brak Player Start – ustaw przy tym portalu (stare zachowanie)
		PlayerPawn->SetActorLocationAndRotation(GetActorLocation(), GetActorRotation());
	}

	// Od razu + z opóźnieniem – przy starcie poziomu pierwsze wywołanie bywa za wcześnie (poziom jeszcze nie „posiada” postaci)
	RestoreGameInputAfterPortalTravel();
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(RestoreInputTimerHandle, this, &APortal::RestoreGameInputAfterPortalTravel, 0.15f, false);
		World->GetTimerManager().SetTimer(RestoreInputTimerHandle2, this, &APortal::RestoreGameInputAfterPortalTravel, 0.5f, false);
	}

	GI->PendingTeleportPortalTag = NAME_None;
}

void APortal::RestoreGameInputAfterPortalTravel()
{
	if (!GetWorld()) return;

	if (ALabaITPlayerController* LabaPC = Cast<ALabaITPlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		LabaPC->RestoreGameOnlyInput();
	}
	else if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		PC->SetShowMouseCursor(false);
		PC->SetIgnoreMoveInput(false);
		PC->SetIgnoreLookInput(false);
		FInputModeGameOnly InputMode;
		InputMode.SetConsumeCaptureMouseDown(true);
		PC->SetInputMode(InputMode);
		PC->FlushPressedKeys();
	}
}
