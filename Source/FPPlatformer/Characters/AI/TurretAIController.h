// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIModule/Classes/BehaviorTree/BehaviorTree.h"
#include "TurretAIController.generated.h"

/**
 * 
 */
UCLASS()
class FPPLATFORMER_API ATurretAIController : public AAIController
{
	GENERATED_BODY()

public:
	ATurretAIController();

	virtual void Possess(APawn* Inpawn) override;

	UPROPERTY(transient)
		UBlackboardComponent* BlackboardComponent;

	UPROPERTY(transient)
		UBehaviorTreeComponent* BehaviourTreeComponent;
	
};
