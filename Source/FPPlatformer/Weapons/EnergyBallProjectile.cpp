// Fill out your copyright notice in the Description page of Project Settings.

#include "EnergyBallProjectile.h"
#include "Runtime/Engine/Classes/Engine/World.h"

// Sets default values
AEnergyBallProjectile::AEnergyBallProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CharacterHealth = CreateDefaultSubobject<UCharacterHealthComponent>(TEXT("CharacterHealth"));
	CharacterHealth->MaxHealth = 1;
	CharacterHealth->PhysicalResistance = 1.0f;
	CharacterHealth->ExplosiveResistance = 1.0f;
	CharacterHealth->FireResistance = 1.0f;
	CharacterHealth->IceResistance = 1.0f;
	CharacterHealth->EnergyResistance = 0.0f;

	CharacterHealth->KillCharacter.AddDynamic(this, &AEnergyBallProjectile::Die);
}

void AEnergyBallProjectile::Die()
{
	FActorSpawnParameters SpawnParams;

	GetWorld()->SpawnActor<AExplosion>(Explosion, GetActorLocation(), GetActorRotation(), SpawnParams);
	Destroy();
}
