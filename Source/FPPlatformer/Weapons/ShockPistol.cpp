// Fill out your copyright notice in the Description page of Project Settings.

#include "ShockPistol.h"
#include "Runtime/Engine/Classes/Components/LineBatchComponent.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"

// Sets default values
AShockPistol::AShockPistol()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AShockPistol::BeginPlay()
{
	Super::BeginPlay();

	IsAltOnCooldown = false;
}

void AShockPistol::OnFirePressed()
{
	if (!IsOnCooldown)
	{
		IsOnCooldown = true;
		GetWorldTimerManager().SetTimer(CooldownTimerHandle, this, &AShockPistol::EndCooldown, FireCooldown, false, FireCooldown);

		FHitResult outHit;
		bool isHit = false;

		TArray<AActor*> actorsToIgnore;
		actorsToIgnore.Add(GetParentActor());

		isHit = UKismetSystemLibrary::SphereTraceSingle(this, GetActorLocation() + GetActorRotation().RotateVector(MuzzleOffset), LookedTarget, 0.5f, UEngineTypes::ConvertToTraceType(ECC_Visibility), false, actorsToIgnore, EDrawDebugTrace::None, outHit, true, FLinearColor::Red, FLinearColor::Green, 0.0f);

		if (isHit)
		{
			UCharacterHealthComponent* otherHP = Cast<UCharacterHealthComponent>(outHit.Actor->GetComponentByClass(UCharacterHealthComponent::StaticClass()));
			if (otherHP)
			{
				otherHP->Damage(PrimaryDamageType, PrimaryDamage);
			}

			//GetWorld()->LineBatcher->DrawLine(GetActorLocation() + GetActorRotation().RotateVector(MuzzleOffset), outHit.ImpactPoint, FLinearColor(FColor::Green), SDPG_World, 2.0f, 1000.0f);
		}

		else
		{
			//GetWorld()->LineBatcher->DrawLine(GetActorLocation() + GetActorRotation().RotateVector(MuzzleOffset), LookedTarget, FLinearColor(FColor::Green), SDPG_World, 1, 1.0f);
		}
		ShootProjectile(FireProjectile, FireProjectileLaunchSpeed);
		if(PrimaryFireShake)
			GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayCameraShake(PrimaryFireShake, 1.0f);
	}
}

void AShockPistol::OnAltFirePressed()
{
	if (!IsAltOnCooldown)
	{
		ShootProjectile(AltFireProjectile, AltFireProjectileLaunchSpeed);
		IsAltOnCooldown = true;
		GetWorldTimerManager().SetTimer(AltCooldownTimerHandle, this, &AShockPistol::EndAltCooldown, AltFireCooldown, true, AltFireCooldown);
		if(SecondaryFireShake)
			GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayCameraShake(SecondaryFireShake, 1.0f);
	}
}

void AShockPistol::EndCooldown()
{
	IsOnCooldown = false;
	GetWorldTimerManager().ClearTimer(CooldownTimerHandle);
}

void AShockPistol::EndAltCooldown()
{
	IsAltOnCooldown = false;
	GetWorldTimerManager().ClearTimer(AltCooldownTimerHandle);
}

void AShockPistol::Destroyed()
{
	Super::Destroyed();
	GetWorldTimerManager().ClearTimer(CooldownTimerHandle);
	GetWorldTimerManager().ClearTimer(AltCooldownTimerHandle);
}

void AShockPistol::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	GetWorldTimerManager().ClearTimer(CooldownTimerHandle);
	GetWorldTimerManager().ClearTimer(AltCooldownTimerHandle);
}
