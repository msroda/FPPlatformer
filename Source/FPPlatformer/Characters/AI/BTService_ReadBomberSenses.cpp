// Fill out your copyright notice in the Description page of Project Settings.

#include "BTService_ReadBomberSenses.h"
#include "BomberAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BomberCharacter.h"

UBTService_ReadBomberSenses::UBTService_ReadBomberSenses()
{
	bCreateNodeInstance = true;
}

void UBTService_ReadBomberSenses::TickNode(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	ABomberAIController* controller = Cast<ABomberAIController>(OwnerComp.GetAIOwner());

	if (controller)
	{
		ABomberCharacter *character = Cast<ABomberCharacter>(controller->GetPawn());
		if (character)
		{
			if(character->TargetPawn)
				OwnerComp.GetBlackboardComponent()->SetValueAsObject("TargetActor", character->TargetPawn);
			if (character->CanSeePlayer)
				OwnerComp.GetBlackboardComponent()->SetValueAsVector("TargetLocation", character->LastPlayersLocation);
			OwnerComp.GetBlackboardComponent()->SetValueAsBool("CanSeePlayer", character->CanSeePlayer);
			OwnerComp.GetBlackboardComponent()->SetValueAsBool("IsAlerted", character->IsAlerted);
			OwnerComp.GetBlackboardComponent()->SetValueAsBool("IsExploding", character->IsExploding);
		}
	}
}
