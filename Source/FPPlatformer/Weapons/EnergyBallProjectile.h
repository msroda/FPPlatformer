// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/BaseProjectile.h"
#include "Weapons/Explosion.h"
#include "Characters/CharacterHealthComponent.h"
#include "EnergyBallProjectile.generated.h"

/**
 * 
 */
UCLASS()
class FPPLATFORMER_API AEnergyBallProjectile : public ABaseProjectile
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEnergyBallProjectile();

	UFUNCTION()
		void Die();
	
	// Character's health component
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UCharacterHealthComponent* CharacterHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AExplosion> Explosion;
};
