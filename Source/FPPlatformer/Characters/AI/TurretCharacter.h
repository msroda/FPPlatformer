// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AIModule/Classes/BehaviorTree/BehaviorTree.h"
#include "Weapons/Explosion.h"
#include "Weapons/BaseProjectile.h"
#include "AIModule/Classes/Perception/PawnSensingComponent.h"
#include "TurretCharacter.generated.h"

UCLASS()
class FPPLATFORMER_API ATurretCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATurretCharacter();

	UPROPERTY(EditAnywhere)
		UBehaviorTree* Behavior;

	// Character's health component
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UCharacterHealthComponent* CharacterHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
		UParticleSystem* FireParticleTemplate;

	UPROPERTY()
		UParticleSystemComponent* FireParticleSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Senses)
		UPawnSensingComponent* PawnSensingComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Senses)
		float PlayerLoseTime = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Senses)
		float AlertTime = 5.0f;

	UFUNCTION()
		void ReceiveDamage(float damage, EDamageType damageType);

	UFUNCTION()
		void OnDamageEventEnded(EDamageType damageType);

	UFUNCTION()
		void OnSeePlayer(APawn* Pawn);

	UFUNCTION()
		void Die();

	APawn* TargetPawn = nullptr;

	bool IsAlerted = false;

	bool CanSeePlayer = false;

	bool CanShoot = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gun)
		FVector MuzzleOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gun)
		float ProjectileVelocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gun)
		float ShootingCooldown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gun)
		TSubclassOf<ABaseProjectile> Projectile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gun)
		bool PredictTargetPosition = false;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Handler for timer to set off an explosion
	FTimerHandle ShootCooldownTimer;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// For clearing timers when bullet is destroyed
	virtual void Destroyed() override;

	// For clearing timers when game stops
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	bool ShootPlayer();

	void EndCooldown();

private:

	float AlertTimer = 0.0f;
};
