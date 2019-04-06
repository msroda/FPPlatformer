// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_SetRandomLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIModule/Classes/AIController.h"

EBTNodeResult::Type UBTTask_SetRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* character = OwnerComp.GetAIOwner()->GetPawn();

	if (character)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector("TargetLocation", FVector(FMath::RandRange(-200.0f, 200.f), FMath::RandRange(-200.0f, 200.f), 0.0f) + character->GetActorLocation());
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}