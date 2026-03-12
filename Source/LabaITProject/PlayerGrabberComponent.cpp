// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerGrabberComponent.h"

// Sets default values for this component's properties
UPlayerGrabberComponent::UPlayerGrabberComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UPlayerGrabberComponent::BeginPlay()
{
	Super::BeginPlay();

	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle)
	{
		UE_LOG(LogTemp, Warning, TEXT("PhysicsHandleComponent required"));
	}

	APlayerController* PC = GetWorld()->GetFirstPlayerController();							//  GetWorld()-> Swiat aktualnie otworzony GetFirstPlayerController()-> logika ktÛra kontroluje gracza
	//UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PC->InputComponent);		// poniewaz kazda klasa ma podpiety InputComponent to mozemy zrobic "PC->InputComponent". 
	// castujemy przestarza≥y InputComponent na EnhancedInputComponent, ktÛry dziedziczy po InputComponent --- castowanie w≥asnie moze sie tylko powieúÊ gdy wystepuje wspomniane dziedziczenie

	//PRACA ZE WSKAèNIKAMI -
	//np. co jezeli castowanie siÍ nie powiedzie?
	// Przy pracy ze wskaünikami trzeba dbaÊ o to by sprawdzaÊ czy wskaünik nie jest null

	/* 3 SPOSOBY
	if (EIC != nullptr) {

	}

	if (EIC) {

	}

	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PC->InputComponent)) {

	}*/

	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PC->InputComponent))
	{
		if (GrabAction)		// czy GrabAction jest rzeczywiúcie podpiÍte
		{
			EIC->BindAction(GrabAction, ETriggerEvent::Started, this, &UPlayerGrabberComponent::Use);   // Bindujemy akcje do funkcji - jeøeli wywo≥a sie akcja to wywo≥a siÍ funkcja
		}
		//ETriggerEvent --> E oznacza w unrealu Enum
	}
}

// Called every frame
void UPlayerGrabberComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PhysicsHandle && PhysicsHandle->GetGrabbedComponent())
	{
		FVector Start;
		FVector End;
		FRotator CameraRotation;
		GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(Start, CameraRotation);
		End = Start + CameraRotation.Vector() * GrabRange;			// aby by≥o wmiare wiarygodnie trzeba by bylo wzglÍdnπ rotacjÍ obiektu ale na razie tutaj trzymamy na sztywno odstÍp - 2 metry przed graczem

		PhysicsHandle->SetTargetLocationAndRotation(End, CameraRotation);
	}
}

void UPlayerGrabberComponent::Use()
{
	if (PhysicsHandle)
	{
		if (PhysicsHandle->GetGrabbedComponent()) //
		{
			Release();
		}
		else
		{
			Grab();
		}
	}
}

void UPlayerGrabberComponent::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("UPlayerGrabberComponent::Grab"));

	FVector Start;
	FVector End;
	FRotator CameraRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(Start, CameraRotation);
	End = Start + CameraRotation.Vector() * GrabRange;

	FHitResult Result; // "F" w FHitResultoznacza "stuktura"
	GetWorld()->LineTraceSingleByChannel(Result, Start, End, ECollisionChannel::ECC_Visibility); // widzimy tylko te obiekty, ktÛre sπ widoczne

	//DrawDebugLine(GetWorld(), Start, End, FColor::Cyan, false, 5.0f, 0, 5.0f);

	if (UPrimitiveComponent* PC = Result.GetComponent())	//GetActor z≥apie nam ca≥y komponent a getcomponent jest troche bardziej dok≥adny				 
	{
		UE_LOG(LogTemp, Warning, TEXT("Grabbed component: %s"), *PC->GetName());

		if (PhysicsHandle)	//Bez tego mamy error po prÛbie podniesienia --> Access violation reading location 0x0000000000000000. ---> dlatego trzeba sprawdzaÊ czy nie jest null --> typowe ryzyko korzystania z pointerÛw
		{
			PhysicsHandle->GrabComponentAtLocationWithRotation(PC, NAME_None, Result.Location, CameraRotation);
		}
	}
}

void UPlayerGrabberComponent::Release()
{
	PhysicsHandle->ReleaseComponent();
}