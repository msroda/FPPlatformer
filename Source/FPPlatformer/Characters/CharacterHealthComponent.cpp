// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterHealthComponent.h"
#include "Engine/Classes/Engine/World.h"

// Sets default values for this component's properties
UCharacterHealthComponent::UCharacterHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	Resistances.SetNumZeroed((int)EDamageType::DMG_MAX, false);
	EventsDamageOnTick.SetNumZeroed((int)EDamageType::DMG_MAX, false);
	EventsDuration.SetNumZeroed((int)EDamageType::DMG_MAX, false);
	CurrentTicks.SetNumZeroed((int)EDamageType::DMG_MAX, false);
}


void UCharacterHealthComponent::OvertimeEventsTick()
{
	for (int i = 0; i < (int)EDamageType::DMG_MAX; i++)
	{
		if (CurrentTicks[i] > 0)
		{
			CurrentTicks[i]--;
			CurrentHealth -= EventsDamageOnTick[i];
			if (CurrentHealth == 0.0f)
			{
				KillCharacter.Broadcast();
			}
			if (CurrentTicks[i] == 0)
			{
				OvertimeEventEnded.Broadcast((EDamageType)i);
			}
		}
	}
}

// Called when the game starts
void UCharacterHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	RegenTimer = 0.0f;
	Rebirth();
}


// Called every frame
void UCharacterHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (RegenTimer < RegenerationCooldown)
		RegenTimer += DeltaTime;
	else
	{
		if (CurrentHealth < MaxHealth)
		{
			CurrentHealth = FMath::FInterpConstantTo(CurrentHealth, MaxHealth, DeltaTime, RegenerationRate);
		}
	}
}

void UCharacterHealthComponent::Damage(EDamageType dmgType, float value)
{
	CurrentHealth = FMath::Max(CurrentHealth - value * (1 - Resistances[(int)dmgType]), 0.0f);
	if (CurrentHealth == 0.0f)
	{
		Kill();
	}

	if (value > 0.0f)
	{
		CurrentTicks[(int)dmgType] = EventsDuration[(int)dmgType];

		RegenTimer = 0.0f;
		DamageCharacter.Broadcast(value, dmgType);
	}
}

void UCharacterHealthComponent::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	Super::OnComponentDestroyed(bDestroyingHierarchy);
	//GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
}

void UCharacterHealthComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetWorld()->GetTimerManager().ClearTimer(OvertimeEventsTicker);
	Super::EndPlay(EndPlayReason);
}

void UCharacterHealthComponent::Rebirth()
{
	CurrentHealth = MaxHealth;
	GetWorld()->GetTimerManager().SetTimer(OvertimeEventsTicker, this, &UCharacterHealthComponent::OvertimeEventsTick, 1 / TicksPerSecond, true, 1 / TicksPerSecond);
}

void UCharacterHealthComponent::Kill()
{
	KillCharacter.Broadcast();
	GetWorld()->GetTimerManager().ClearTimer(OvertimeEventsTicker);
}

