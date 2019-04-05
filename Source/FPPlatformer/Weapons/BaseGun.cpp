// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseGun.h"
#include "Weapons/BaseProjectile.h"
#include "Runtime/Engine/Classes/Components/PrimitiveComponent.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Engine/Public/DrawDebugHelpers.h"

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
			UPrimitiveComponent* bulletPrimitive = projectile->FindComponentByClass<UPrimitiveComponent>();

			AActor* myparent = GetParentActor();

			if (myparent && bulletPrimitive)
			{
				bulletPrimitive->IgnoreActorWhenMoving(myparent, true);
				projectile->Shooter = myparent;
			}

			FVector velocity = (target - (GetActorLocation() + MuzzleOffset)).GetUnsafeNormal() * speed;

			projectile->SetVelocity(velocity);
		}
	}
}

bool ABaseGun::ShootHitscan(FVector target, FHitResult & outHit)
{
	TArray<FHitResult> outHits;
	FHitResult* tempHit = nullptr;
	bool isHit = false;

	FCollisionQueryParams collisionParams;

	isHit = GetWorld()->LineTraceMultiByChannel(outHits, GetActorLocation() + MuzzleOffset, target, ECC_Visibility, collisionParams);

	//ignore self collision
	for (auto hit : outHits)
	{
		if (hit.Actor != GetParentActor())
		{
			tempHit = &hit;
			break;
		}
	}

	if (isHit && tempHit)
	{
		outHit = *tempHit;
		return true;
	}

	else
	{
		return false;
	}
}

// Called every frame
void ABaseGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

