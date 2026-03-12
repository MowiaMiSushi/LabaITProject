// Fill out your copyright notice in the Description page of Project Settings.

#include "HealthStaminaComponent.h"

UHealthStaminaComponent::UHealthStaminaComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UHealthStaminaComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;
	CurrentStamina = MaxStamina;
	TimeSinceLastStaminaUse = 0.0f;

	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);
	OnStaminaChanged.Broadcast(CurrentStamina, MaxStamina);
}

void UHealthStaminaComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Regeneracja staminy
	if (StaminaRegenPerSecond > 0.0f && CurrentStamina < MaxStamina)
	{
		TimeSinceLastStaminaUse += DeltaTime;
		if (TimeSinceLastStaminaUse >= StaminaRegenDelay)
		{
			CurrentStamina = FMath::Min(MaxStamina, CurrentStamina + StaminaRegenPerSecond * DeltaTime);
			OnStaminaChanged.Broadcast(CurrentStamina, MaxStamina);
		}
	}
}

void UHealthStaminaComponent::TakeDamage(float Amount)
{
	if (Amount <= 0.0f || !IsAlive()) return;

	CurrentHealth = FMath::Max(0.0f, CurrentHealth - Amount);
	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);

	if (!IsAlive())
	{
		OnDeath.Broadcast();
	}
}

void UHealthStaminaComponent::Heal(float Amount)
{
	if (Amount <= 0.0f) return;

	CurrentHealth = FMath::Min(MaxHealth, CurrentHealth + Amount);
	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);
}

float UHealthStaminaComponent::GetHealthPercent() const
{
	return MaxHealth > 0.0f ? (CurrentHealth / MaxHealth) : 0.0f;
}

bool UHealthStaminaComponent::ConsumeStamina(float Amount)
{
	if (Amount <= 0.0f || CurrentStamina < Amount) return false;

	CurrentStamina -= Amount;
	TimeSinceLastStaminaUse = 0.0f;
	OnStaminaChanged.Broadcast(CurrentStamina, MaxStamina);
	return true;
}

void UHealthStaminaComponent::AddStamina(float Amount)
{
	if (Amount <= 0.0f) return;

	CurrentStamina = FMath::Min(MaxStamina, CurrentStamina + Amount);
	OnStaminaChanged.Broadcast(CurrentStamina, MaxStamina);
}

float UHealthStaminaComponent::GetStaminaPercent() const
{
	return MaxStamina > 0.0f ? (CurrentStamina / MaxStamina) : 0.0f;
}
