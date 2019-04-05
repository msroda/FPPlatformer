// Fill out your copyright notice in the Description page of Project Settings.

#include "BTService_ReadTurretSenses.h"
#include "Characters/AI/TurretAIController.h"
#include "Characters/AI/TurretCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_ReadTurretSenses::UBTService_ReadTurretSenses()
{
	bCreateNodeInstance = true;
}

void UBTService_ReadTurretSenses::TickNode(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	ATurretAIController* controller = Cast<ATurretAIController>(OwnerComp.GetAIOwner());

	if (controller)
	{
		ATurretCharacter *character = Cast<ATurretCharacter>(controller->GetPawn());
		if (character)
		{
			if (character->TargetPawn)
				OwnerComp.GetBlackboardComponent()->SetValueAsObject("TargetActor", character->TargetPawn);
			OwnerComp.GetBlackboardComponent()->SetValueAsBool("CanSeePlayer", character->CanSeePlayer);
			OwnerComp.GetBlackboardComponent()->SetValueAsBool("CanShoot", character->CanShoot);
		}
	}
}
