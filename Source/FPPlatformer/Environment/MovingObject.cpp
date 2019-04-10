// Fill out your copyright notice in the Description page of Project Settings.

#include "MovingObject.h"

// Sets default values
AMovingObject::AMovingObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SwitchableComponent = CreateDefaultSubobject<USwitchableComponent>(TEXT("Switchable"));
	SwitchableComponent->SwitchObjectState.AddDynamic(this, &AMovingObject::Switch);

}

// Called when the game starts or when spawned
void AMovingObject::BeginPlay()
{
	Super::BeginPlay();
	
	StartPosition = GetActorLocation();
	DeltaPosition = 0.0f;
	Direction = false;
	
	if (Target)
	{
		TargetPosition = Target->GetActorLocation();
		Target->Destroy();
	}
	else
	{
		TargetPosition = GetActorLocation();
	}
}

// Called every frame
void AMovingObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if ((Direction && DeltaPosition == 1.0f) || (!Direction && DeltaPosition == 0.0f))
		return;

	if (Direction)
	{
		DeltaPosition = FMath::Min(DeltaPosition + DeltaTime / MovingTime, 1.0f);
	}
	else
	{
		DeltaPosition = FMath::Max(DeltaPosition - DeltaTime / MovingTime, 0.0f);
	}

	SetActorLocation(StartPosition * (1 - DeltaPosition) + TargetPosition * DeltaPosition, true);
}

void AMovingObject::Switch()
{
	Direction = !Direction;
}

