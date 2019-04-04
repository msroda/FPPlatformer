// Fill out your copyright notice in the Description page of Project Settings.

#include "BomberAIController.h"
#include "BomberCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

ABomberAIController::ABomberAIController()
{
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));

	BehaviourTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
}

void ABomberAIController::Possess(APawn * Inpawn)
{
	Super::Possess(Inpawn);

	ABomberCharacter* character = Cast<ABomberCharacter>(Inpawn);

	if (character && character->Behavior)
	{
		BlackboardComponent->InitializeBlackboard(*character->Behavior->BlackboardAsset);

		BehaviourTreeComponent->StartTree(*character->Behavior);

	}
}