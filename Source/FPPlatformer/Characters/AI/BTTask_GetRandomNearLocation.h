// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_GetRandomNearLocation.generated.h"

/**
 * 
 */
UCLASS()
class FPPLATFORMER_API UBTTask_GetRandomNearLocation : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UPROPERTY(Category = Node, EditAnywhere)
		float Radius = 300;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory) override;

};
