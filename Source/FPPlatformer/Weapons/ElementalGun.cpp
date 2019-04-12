// Fill out your copyright notice in the Description page of Project Settings.

#include "ElementalGun.h"

AElementalGun::AElementalGun()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AElementalGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PlayerOwner)
	{
		if (IsFiring && !IsOnCooldown)
		{
			ShootProjectile(FireProjectile, FireProjectileLaunchSpeed);
			IsOnCooldown = true;
			GetWorldTimerManager().SetTimer(CooldownTimerHandle, this, &AElementalGun::EndCooldown, FireCooldown, false, FireCooldown);
			if (CameraShake)
				GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayCameraShake(CameraShake, 1.0f);
		}
		else if (IsAltFiring && !IsOnCooldown)
		{
			ShootProjectile(AltFireProjectile, AltFireProjectileLaunchSpeed);
			IsOnCooldown = true;
			GetWorldTimerManager().SetTimer(CooldownTimerHandle, this, &AElementalGun::EndCooldown, AltFireCooldown, false, AltFireCooldown);
			if (CameraShake)
				GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayCameraShake(CameraShake, 1.0f);
		}

	}
}

void AElementalGun::BeginPlay()
{
	Super::BeginPlay();

	IsOnCooldown = false;
	IsFiring = false;
	IsAltFiring = false;

	PlayerOwner = Cast<AMyCharacter>(GetParentActor());
}

void AElementalGun::OnFirePressed()
{
	if (!IsAltFiring)
		IsFiring = true;
}

void AElementalGun::OnFireReleased()
{
	IsFiring = false;
}

void AElementalGun::OnAltFirePressed()
{
	if (!IsFiring)
		IsAltFiring = true;
}

void AElementalGun::OnAltFireReleased()
{
	IsAltFiring = false;
}

void AElementalGun::EndCooldown()
{
	IsOnCooldown = false;
	GetWorldTimerManager().ClearTimer(CooldownTimerHandle);
}

void AElementalGun::Destroyed()
{
	Super::Destroyed();
	GetWorldTimerManager().ClearTimer(CooldownTimerHandle);
}

void AElementalGun::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	GetWorldTimerManager().ClearTimer(CooldownTimerHandle);
}

void AElementalGun::SetActive(bool active)
{
	IsFiring = false;
	IsAltFiring = false;
	
	Super::SetActive(active);	
}
