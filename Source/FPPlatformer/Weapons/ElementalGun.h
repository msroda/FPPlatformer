// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/BaseGun.h"
#include "Weapons/BaseProjectile.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "Characters/Player/MyCharacter.h"
#include "ElementalGun.generated.h"

/**
 * 
 */
UCLASS()
class FPPLATFORMER_API AElementalGun : public ABaseGun
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AElementalGun();

	/** Projectile to spawn on fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectiles)
		TSubclassOf<ABaseProjectile> FireProjectile;

	/** Projectile to spawn on alt fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectiles)
		TSubclassOf<ABaseProjectile> AltFireProjectile;

	/** Velocity applied to projectile on spawn */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectiles)
		float FireProjectileLaunchSpeed = 1000;

	/** Velocity applied to projectile on spawn */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectiles)
		float AltFireProjectileLaunchSpeed = 1000;

	/** Primary fire cooldown */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectiles)
		float FireCooldown = 0.2f;

	/** Secondary fire cooldown */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectiles)
		float AltFireCooldown = 0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectiles)
		TSubclassOf<UCameraShake> CameraShake;

	virtual void Tick(float DeltaTime) override;

protected:

	/** Handler for primary fire cooldown */
	FTimerHandle CooldownTimerHandle;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Primary fire start */
	virtual void OnFirePressed() override;

	/** Primary fire end */
	virtual void OnFireReleased() override;

	/** Secondary fire start */
	virtual void OnAltFirePressed() override;

	/** Secondary fire start */
	virtual void OnAltFireReleased() override;

	/** Primary fire's cooldown end */
	void EndCooldown();

public:
	/** For clearing timers when bullet is destroyed */
	virtual void Destroyed() override;

	/** For clearing timers when game stops */
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void SetActive(bool active) override;

private:
	//Can alt fire
	bool IsOnCooldown;

	//Is firing
	bool IsFiring;

	//Is firing
	bool IsAltFiring;

	//Parent
	AMyCharacter* PlayerOwner;
};
