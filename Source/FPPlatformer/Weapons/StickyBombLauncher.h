// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/BaseGun.h"
#include "Weapons/BaseProjectile.h"
#include "Weapons/StickyBombProjectile.h"
#include "StickyBombLauncher.generated.h"

/**
 * 
 */
UCLASS()
class FPPLATFORMER_API AStickyBombLauncher : public ABaseGun
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectiles)
		TSubclassOf<ABaseProjectile> Projectile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectiles)
		float ProjectileLaunchSpeed = 600;

	/** Velocity applied to projectile on spawn */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectiles)
		float FireCooldown = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectiles)
		int SpawnedBombsLimit = 5;

protected:

	FTimerHandle CooldownTimerHandle;

	TArray<AStickyBombProjectile*> Bombs;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Primary fire start */
	virtual void OnFirePressed() override;

	/** Secondary fire start */
	virtual void OnAltFirePressed() override;

	/** Primary fire's cooldown end */
	void EndCooldown();

public:

	/** For clearing timers when bullet is destroyed */
	virtual void Destroyed() override;

	/** For clearing timers when game stops */
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:

	bool IsOnCooldown;
};
