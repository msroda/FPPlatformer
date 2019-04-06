// Fill out your copyright notice in the Description page of Project Settings.

#include "StickyBombLauncher.h"
#include "Engine/Public/TimerManager.h"

void AStickyBombLauncher::BeginPlay()
{
	Super::BeginPlay();

	IsOnCooldown = false;
}

void AStickyBombLauncher::OnFirePressed(FVector target)
{
	if (!IsOnCooldown && Bombs.Num() < SpawnedBombsLimit)
	{
		AStickyBombProjectile* projectile = Cast<AStickyBombProjectile>(ShootProjectile(target, Projectile, ProjectileLaunchSpeed));
		IsOnCooldown = true;
		GetWorldTimerManager().SetTimer(CooldownTimerHandle, this, &AStickyBombLauncher::EndCooldown, FireCooldown, true, FireCooldown);
		if (projectile)
		{
			Bombs.Add(projectile);
		}
	}
}

void AStickyBombLauncher::OnAltFirePressed(FVector target)
{
	for (auto bomb : Bombs)
	{
		bomb->Explode();	
	}

	Bombs.Empty();
}

void AStickyBombLauncher::EndCooldown()
{
	IsOnCooldown = false;
	GetWorldTimerManager().ClearTimer(CooldownTimerHandle);
}

void AStickyBombLauncher::Destroyed()
{
	Super::Destroyed();
	GetWorldTimerManager().ClearTimer(CooldownTimerHandle);
}

void AStickyBombLauncher::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	GetWorldTimerManager().ClearTimer(CooldownTimerHandle);
}
