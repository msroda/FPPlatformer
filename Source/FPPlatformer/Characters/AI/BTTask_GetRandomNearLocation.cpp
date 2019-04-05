// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_GetRandomNearLocation.h"
#include "AIModule/Classes/AIController.h"
#include "GameFramework/Pawn.h"
#include "Runtime/NavigationSystem/Public/NavigationSystem.h"
#include "BomberAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTTask_GetRandomNearLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ABomberAIController* controller = Cast<ABomberAIController>(OwnerComp.GetAIOwner());

	if (controller)
	{
		APawn* pawn = controller->GetPawn();

		if (pawn)
		{
			FVector newLocation;
			bool result = UNavigationSystemV1::K2_GetRandomReachablePointInRadius(pawn, pawn->GetActorLocation(), newLocation, Radius);

			if (result)
			{
				controller->BlackboardComponent->SetValueAsVector("TargetLocation", newLocation);
				return EBTNodeResult::Succeeded;
			}
		}

	}
	return EBTNodeResult::Failed;
}