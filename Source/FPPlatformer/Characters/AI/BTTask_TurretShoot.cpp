// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_TurretShoot.h"
#include "TurretAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "TurretCharacter.h"

EBTNodeResult::Type UBTTask_TurretShoot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ATurretAIController* controller = Cast<ATurretAIController>(OwnerComp.GetAIOwner());

	ATurretCharacter* character = Cast<ATurretCharacter>(controller->GetPawn());

	if (character && character->ShootPlayer())
	{
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}