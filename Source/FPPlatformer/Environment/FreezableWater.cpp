// Fill out your copyright notice in the Description page of Project Settings.

#include "FreezableWater.h"
#include "Engine/Classes/Components/StaticMeshComponent.h"

// Sets default values
AFreezableWater::AFreezableWater()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HealthComponent = CreateDefaultSubobject<UCharacterHealthComponent>(TEXT("CharacterHealth"));
	HealthComponent->MaxHealth = 100;
	HealthComponent->PhysicalResistance = 1.0f;
	HealthComponent->ExplosiveResistance = 1.0f;
	HealthComponent->FireResistance = 1.0f;
	HealthComponent->IceResistance = 1.0f;
	HealthComponent->EnergyResistance = 1.0f;

	HealthComponent->DamageCharacter.AddDynamic(this, &AFreezableWater::ReceiveDamage);

}

void AFreezableWater::ReceiveDamage(float damage, EDamageType damageType)
{
	if (IsIce)
	{
		if (damageType == EDamageType::DMG_Fire)
		{
			SetActorEnabled(false);
			if (PairActor)
				PairActor->SetActorEnabled(true);
		}
	}
	else
	{
		if (damageType == EDamageType::DMG_Ice)
		{
			SetActorEnabled(false);
			if (PairActor)
				PairActor->SetActorEnabled(true);
		}
	}
}

// Called when the game starts or when spawned
void AFreezableWater::BeginPlay()
{
	Super::BeginPlay();

	PrimitiveComponent = FindComponentByClass<UPrimitiveComponent>();
	SetActorEnabled(IsActive);
}

void AFreezableWater::SetStateToIce()
{

}

void AFreezableWater::SetStateToWater()
{

}

// Called every frame
void AFreezableWater::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFreezableWater::SetActorEnabled(bool enabled)
{
	SetActorHiddenInGame(!enabled);

	SetActorEnableCollision(enabled);

	SetActorTickEnabled(enabled);
}

