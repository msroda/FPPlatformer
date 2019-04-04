// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Runtime/AIModule/Classes/BehaviorTree/BehaviorTreeComponent.h"
#include "BomberAIController.generated.h"

UCLASS()
class FPPLATFORMER_API ABomberAIController : public AAIController
{
	GENERATED_BODY()

public:
	ABomberAIController();

	virtual void Possess(APawn* Inpawn) override;

	UPROPERTY(transient)
		UBlackboardComponent* BlackboardComponent;

	UPROPERTY(transient)
		UBehaviorTreeComponent* BehaviourTreeComponent;

};
