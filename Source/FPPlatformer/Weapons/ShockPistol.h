// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/BaseGun.h"
#include "Weapons/BaseProjectile.h"
#include "ShockPistol.generated.h"


UCLASS()
class FPPLATFORMER_API AShockPistol : public ABaseGun
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AShockPistol();

	/** Projectile to spawn on alt fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectiles)
		TSubclassOf<ABaseProjectile> AltFireProjectile;

	/** Velocity applied to projectile on spawn */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectiles)
		float AltFireProjectileLaunchSpeed = 600;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Primary fire start */
	virtual void OnFirePressed(FVector target) override;

	/** Secondary fire start */
	virtual void OnAltFirePressed(FVector target) override;
};
