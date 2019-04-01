// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterHealthComponent.h"

// Sets default values for this component's properties
UCharacterHealthComponent::UCharacterHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCharacterHealthComponent::BeginPlay()
{
	Super::BeginPlay();


	CurrentHealth = MaxHealth;
	// ...
	
}


// Called every frame
void UCharacterHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UCharacterHealthComponent::Damage(EDamageType dmgType, float value)
{
	switch (dmgType)
	{
	case EDamageType::DMG_Physical:
		MaxHealth -= value * (1 - PhysicalResistance);
		break;

	case EDamageType::DMG_Explosive:
		MaxHealth -= value * (1 - ExplosiveResistance);
		break;

	case EDamageType::DMG_Fire:
		MaxHealth -= value * (1 - FireResistance);
		break;

	case EDamageType::DMG_Ice:
		MaxHealth -= value * (1 - IceResistance);
		break;
	}
}

