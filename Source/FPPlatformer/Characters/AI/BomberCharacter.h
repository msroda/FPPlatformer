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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Patrolling)
		TArray<ATargetPoint*> Waypoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Patrolling)
		int CurrentWaypointID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Patrolling)
		EPatrollingOrder PatrollingOrder;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Explosion)
		float FuseTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Explosion)
		TSubclassOf<AExplosion> Explosion;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Senses)
		UPawnSensingComponent* PawnSensingComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Senses)
		float AlertTime = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Senses)
		float PlayerLoseTime = 1.0f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Handler for timer to set off an explosion */
	FTimerHandle ExplosionCountdownTimer;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	bool GetNextWaypoint(ATargetPoint* &outPoint);

	UFUNCTION()
		void Explode();

	UFUNCTION()
		void Die();

	bool Shoot();

	virtual void Destroyed() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

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
};
