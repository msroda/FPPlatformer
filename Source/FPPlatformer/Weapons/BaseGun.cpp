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

void ABaseGun::OnFirePressed()
{

}

void ABaseGun::OnFireReleased()
{

}


void ABaseGun::OnAltFirePressed()
{

}

void ABaseGun::OnAltFireReleased()
{

}

void ABaseGun::Aim(FVector target, float delta)
{
	SetActorRotation(FMath::RInterpConstantTo(GetActorRotation(), (target - GetActorLocation()).Rotation(), delta, AimingSpeed));
	LookedTarget = target;
}

AActor* ABaseGun::ShootProjectile(TSubclassOf<class AActor> projectileClass, float speed)
{
	UWorld* const World = GetWorld();
	if (World)
	{
		FActorSpawnParameters SpawnParams;

		// spawn the projectile at the muzzle
		ABaseProjectile* projectile = World->SpawnActor<ABaseProjectile>(projectileClass, GetActorLocation() + GetActorRotation().RotateVector(MuzzleOffset), (LookedTarget - GetActorLocation()).Rotation(), SpawnParams);
		if (projectile && projectile->ProjectileMovement)
		{
			UPrimitiveComponent* bulletPrimitive = projectile->FindComponentByClass<UPrimitiveComponent>();

			AActor* myparent = GetParentActor();

			if (myparent && bulletPrimitive)
			{
				bulletPrimitive->IgnoreActorWhenMoving(myparent, true);
				projectile->Shooter = myparent;
			}

			FVector velocity = (LookedTarget - (GetActorLocation() + GetActorRotation().RotateVector(MuzzleOffset))).GetUnsafeNormal() * speed;

			projectile->SetVelocity(velocity);
			return projectile;
		}
	}
	return nullptr;
}

bool ABaseGun::ShootHitscan(FVector target, FHitResult & outHit)
{
	TArray<FHitResult> outHits;
	FHitResult* tempHit = nullptr;
	bool isHit = false;

	FCollisionQueryParams collisionParams;

	isHit = GetWorld()->LineTraceMultiByChannel(outHits, GetActorLocation() + GetActorRotation().RotateVector(MuzzleOffset), target, ECC_Visibility, collisionParams);

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

void ABaseGun::SetActive(bool active)
{
	IsActive = active;	
	SetActorHiddenInGame(!active);
	SetActorTickEnabled(active);
}

// Called every frame
void ABaseGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	

}

