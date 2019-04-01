// Fill out your copyright notice in the Description page of Project Settings.

#include "ShockPistol.h"

// Sets default values
AShockPistol::AShockPistol()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AShockPistol::BeginPlay()
{
	Super::BeginPlay();
}

void AShockPistol::OnFirePressed(FVector target)
{


}

void AShockPistol::OnAltFirePressed(FVector target)
{
	ShootProjectile(target, AltFireProjectile, AltFireProjectileLaunchSpeed);
}
