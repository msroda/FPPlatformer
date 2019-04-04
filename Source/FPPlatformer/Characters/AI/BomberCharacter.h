// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Runtime/AIModule/Classes/BehaviorTree/BehaviorTree.h"
#include "Runtime/Engine/Classes/Engine/TargetPoint.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Patrolling)
		TArray<ATargetPoint*> Waypoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Patrolling)
		int CurrentWaypointID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Patrolling)
		EPatrollingOrder PatrollingOrder;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	bool GetNextWaypoint(ATargetPoint* &outPoint);

private:

	bool direction = true;
};
