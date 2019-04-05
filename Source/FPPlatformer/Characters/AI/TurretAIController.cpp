// Fill out your copyright notice in the Description page of Project Settings.

#include "TurretAIController.h"
#include "TurretCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

ATurretAIController::ATurretAIController()
{
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));

	BehaviourTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
}

void ATurretAIController::Possess(APawn * Inpawn)
{
	Super::Possess(Inpawn);

	ATurretCharacter* character = Cast<ATurretCharacter>(Inpawn);

	if (character && character->Behavior)
	{
		BlackboardComponent->InitializeBlackboard(*character->Behavior->BlackboardAsset);

		BehaviourTreeComponent->StartTree(*character->Behavior);

	}
}