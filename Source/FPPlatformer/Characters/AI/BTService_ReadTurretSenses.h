// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_ReadTurretSenses.generated.h"

/**
 * 
 */
UCLASS()
class FPPLATFORMER_API UBTService_ReadTurretSenses : public UBTService
{
	GENERATED_BODY()

public:

	UBTService_ReadTurretSenses();

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
