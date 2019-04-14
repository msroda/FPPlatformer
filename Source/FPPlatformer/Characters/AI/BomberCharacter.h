// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Runtime/AIModule/Classes/BehaviorTree/BehaviorTree.h"
#include "Runtime/Engine/Classes/Engine/TargetPoint.h"
#include "Weapons/Explosion.h"
#include "Runtime/AIModule/Classes/Perception/PawnSensingComponent.h"
#include "BomberCharacter.generated.h"


UENUM()
enum class EPatrollingOrder
{
	PO_Around,
	PO_BackAndForth,
	PO_Random
};

UCLASS()
class FPPLATFORMER_API ABomberCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABomberCharacter();

	UPROPERTY(EditAnywhere)
		UBehaviorTree* Behavior;

		// Character's health component
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UCharacterHealthComponent* CharacterHealth;

	// Points to visit during patrol
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Patrolling)
		TArray<ATargetPoint*> Waypoints;

	// Current patrolling point to reach
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Patrolling)
		int CurrentWaypointID;

	// Patroling method
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Patrolling)
		EPatrollingOrder PatrollingOrder;

	// How long before bomber explodes after it reached player/lost hp
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Explosion)
		float FuseTime;

	// Explosion actor to spawn
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Explosion)
		TSubclassOf<AExplosion> Explosion;

	// Particle template to visualize fuze time
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Explosion)
		UParticleSystem* SmokeParticleTemplate;

	// Particle system to visualize fuze time
	UPROPERTY()
		UParticleSystemComponent* SmokeParticleSystem;

	// Particle template to visualize character being on fire
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
		UParticleSystem* FireParticleTemplate;

	// Particle system to visualize character being on fire
	UPROPERTY()
		UParticleSystemComponent* FireParticleSystem;

	// Speed multiplier when frozen
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
		float SlowMultiplier;

	// For sensing player
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Senses)
		UPawnSensingComponent* PawnSensingComponent;

	// How long it takes after losing player to go back to patrolling
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Senses)
		float AlertTime = 10.0f;

	// How long it takes to lose player
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Senses)
		float PlayerLoseTime = 1.0f;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Handler for timer to set off an explosion
	FTimerHandle ExplosionCountdownTimer;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Gte next point to reach during patrol
	bool GetNextWaypoint(ATargetPoint* &outPoint);

	// Spawn explosion and destroy character
	UFUNCTION()
		void Explode();

	// Begin dying
	UFUNCTION()
		void Die();

	bool Shoot();

	// To clear timer
	virtual void Destroyed() override;

	// To clear timer
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// On damage received
	UFUNCTION()
		void ReceiveDamage(float damage, EDamageType damageType);

	// To stop being on fire/frozen
	UFUNCTION()
		void OnDamageEventEnded(EDamageType damageType);

	// When character sees player
	UFUNCTION()
		void OnSeePlayer(APawn* Pawn);

	bool CanSeePlayer = false;

	bool IsAlerted = false;

	bool IsExploding = false;

	FVector LastPlayersLocation;

	APawn* TargetPawn = nullptr;

private:
	bool Direction = true;

	float AlertTimer = 0.0f;

	float OriginalWalkSpeed;
};
