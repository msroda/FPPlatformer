// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseGun.h"
#include "Weapons/BaseProjectile.h"
#include "Runtime/Engine/Classes/Engine/World.h"

// Sets default values
ABaseGun::ABaseGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABaseGun::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseGun::OnFirePressed(FVector target)
{

}

void ABaseGun::OnFireReleased(FVector target)
{

}


void ABaseGun::OnAltFirePressed(FVector target)
{

}

void ABaseGun::OnAltFireReleased(FVector target)
{

}

void ABaseGun::ShootProjectile(FVector target, TSubclassOf<class AActor> projectileClass, float speed)
{
	UWorld* const World = GetWorld();
	if (World)
	{
		FActorSpawnParameters SpawnParams;

		// spawn the projectile at the muzzle
		ABaseProjectile* projectile = World->SpawnActor<ABaseProjectile>(projectileClass, GetActorLocation() + MuzzleOffset, GetActorRotation(), SpawnParams);
		if (projectile && projectile->ProjectileMovement)
		{
			projectile->ProjectileMovement->Velocity = ((target - (GetActorLocation() + MuzzleOffset)).GetUnsafeNormal() * speed);
			//projectile->InitialLifeSpan = 1.0f;
			//projectile->ProjectileMovement->InitialSpeed = 100;
		}
	}
}

// Called every frame
void ABaseGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

