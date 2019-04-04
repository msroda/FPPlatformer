// Fill out your copyright notice in the Description page of Project Settings.

#include "BomberCharacter.h"

// Sets default values
ABomberCharacter::ABomberCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABomberCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABomberCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABomberCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

bool ABomberCharacter::GetNextWaypoint(ATargetPoint *& outPoint)
{
	if (Waypoints.Num() != 0)
	{
		if (Waypoints.Num() > 1)
		{
			switch (PatrollingOrder)
			{
			case EPatrollingOrder::PO_Around:
				CurrentWaypointID = (CurrentWaypointID + 1) % Waypoints.Num();
				break;
			case EPatrollingOrder::PO_BackAndForth:
				if (direction)
				{
					CurrentWaypointID = (CurrentWaypointID + 1) % Waypoints.Num();
				}
				else
				{
					CurrentWaypointID = (CurrentWaypointID + Waypoints.Num() - 1) % Waypoints.Num();
				}
				break;
			case EPatrollingOrder::PO_Random:
				int temp = FMath::RandRange(0, Waypoints.Num() - 2);
				if (temp >= CurrentWaypointID)
					temp++;
				CurrentWaypointID = temp;
				break;
			}
		}
		outPoint = Waypoints[CurrentWaypointID];
		return true;
	}

	return false;
}

