// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_ReadBomberSenses.generated.h"

/**
 * 
 */
UCLASS()
class FPPLATFORMER_API UBTService_ReadBomberSenses : public UBTService
{
	GENERATED_BODY()
	
public:

	UBTService_ReadBomberSenses();

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};
