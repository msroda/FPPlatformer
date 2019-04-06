// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/BaseGun.h"
#include "Weapons/BaseProjectile.h"
#include "Characters/CharacterHealthComponent.h"
#include "Runtime/Engine/Public/TimerManager.h"
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

	/** Primary fire cooldown */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectiles)
		float FireCooldown = 0.3f;
	
	/** Secondary fire cooldown */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectiles)
		float AltFireCooldown = 1.0f;

	/** Primary fire damage type*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectiles)
		EDamageType PrimaryDamageType = EDamageType::DMG_Energy;

	/** Primary fire damage*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectiles)
		float PrimaryDamage = 50;


protected:
	
	/** Handler for primary fire cooldown */
	FTimerHandle CooldownTimerHandle;
	
	/** Handler for alt fire cooldown */
	FTimerHandle AltCooldownTimerHandle;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Primary fire start */
	virtual void OnFirePressed(FVector target) override;

	/** Secondary fire start */
	virtual void OnAltFirePressed(FVector target) override;

	/** Primary fire's cooldown end */
	void EndCooldown();
	
	/** Secondary fire's cooldown end */
	void EndAltCooldown();

public:
	/** For clearing timers when bullet is destroyed */
	virtual void Destroyed() override;

	/** For clearing timers when game stops */
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	/** Can alt fire */
	bool IsOnCooldown;

	/** Can alt fire */
	bool IsAltOnCooldown;
};
