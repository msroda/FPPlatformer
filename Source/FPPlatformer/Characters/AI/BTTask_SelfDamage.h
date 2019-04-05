// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Characters/CharacterHealthComponent.h"
#include "BTTask_SelfDamage.generated.h"

/**
 * 
 */
UCLASS()
class FPPLATFORMER_API UBTTask_SelfDamage : public UBTTaskNode
{
	GENERATED_BODY()

public:

	UPROPERTY(Category = Node, EditAnywhere)
		EDamageType DamageType;

	UPROPERTY(Category = Node, EditAnywhere)
		float Damage;
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory) override;

};
