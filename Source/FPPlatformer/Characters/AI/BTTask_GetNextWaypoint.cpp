// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_GetNextWaypoint.h"
#include "Runtime/Engine/Classes/Engine/TargetPoint.h"
#include "BomberAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BomberCharacter.h"


EBTNodeResult::Type UBTTask_GetNextWaypoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ABomberAIController* BomberController = Cast<ABomberAIController>(OwnerComp.GetAIOwner());

	ATargetPoint* newTarget = nullptr;

	ABomberCharacter* BomberCharacter = Cast<ABomberCharacter>(BomberController->GetPawn());

	if (BomberCharacter && BomberCharacter->GetNextWaypoint(newTarget))
	{
		BomberController->BlackboardComponent->SetValueAsObject("CurrentTargetPoint", newTarget);

		if (newTarget)
			return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}