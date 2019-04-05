// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_SelfDamage.h"
#include "AIModule/Classes/AIController.h"
#include "GameFramework/Pawn.h"

EBTNodeResult::Type UBTTask_SelfDamage::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* Controller = Cast<AAIController>(OwnerComp.GetAIOwner());

	if (Controller && Controller->GetPawn())
	{
		UCharacterHealthComponent* CharacterHealth = Controller->GetPawn()->FindComponentByClass<UCharacterHealthComponent>();

		if (CharacterHealth)
		{
			CharacterHealth->Damage(DamageType, Damage);
			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Failed;
}