// Fill out your copyright notice in the Description page of Project Settings.

#include "TurretCharacter.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"

// Sets default values
ATurretCharacter::ATurretCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CharacterHealth = CreateDefaultSubobject<UCharacterHealthComponent>(TEXT("CharacterHealth"));
	CharacterHealth->MaxHealth = 100;
	CharacterHealth->PhysicalResistance = 0.0f;
	CharacterHealth->ExplosiveResistance = -1.0f;
	CharacterHealth->FireResistance = 0.0f;
	CharacterHealth->IceResistance = 0.0f;
	CharacterHealth->EnergyResistance = 0.0f;

	CharacterHealth->KillCharacter.AddDynamic(this, &ATurretCharacter::Die);

	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("CharacterSensing"));
	PawnSensingComponent->OnSeePawn.AddDynamic(this, &ATurretCharacter::OnSeePlayer);
}

void ATurretCharacter::OnSeePlayer(APawn * Pawn)
{
	CanSeePlayer = true;
	IsAlerted = true;
	AlertTimer = 0.0f;
	TargetPawn = Pawn;
}

void ATurretCharacter::Die()
{
	Destroy();
}

// Called when the game starts or when spawned
void ATurretCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATurretCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsAlerted)
	{
		AlertTimer += DeltaTime;

		if (AlertTimer > PlayerLoseTime)
			CanSeePlayer = false;

		if (AlertTimer > AlertTime)
			IsAlerted = false;
	}
}

// Called to bind functionality to input
void ATurretCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ATurretCharacter::Destroyed()
{
	Super::Destroyed();
	GetWorldTimerManager().ClearTimer(ShootCooldownTimer);
}

void ATurretCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	GetWorldTimerManager().ClearTimer(ShootCooldownTimer);
}

bool ATurretCharacter::ShootPlayer()
{
	if (CanShoot && TargetPawn)
	{
		UWorld* const World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;

			ABaseProjectile* projectile = World->SpawnActor<ABaseProjectile>(Projectile, GetActorLocation() + GetActorRotation().RotateVector(MuzzleOffset), GetActorRotation(), SpawnParams);
			if (projectile && projectile->ProjectileMovement)
			{
				UPrimitiveComponent* bulletPrimitive = projectile->FindComponentByClass<UPrimitiveComponent>();

				AActor* myparent = GetParentActor();

				if (bulletPrimitive)
				{
					bulletPrimitive->IgnoreActorWhenMoving(this, true);
					projectile->Shooter = this;
				}

				FVector targetOffset = TargetPawn->GetActorLocation() - (GetActorLocation() + MuzzleOffset); //simple trajectory
				FVector pawnVelocity = TargetPawn->GetVelocity();
				FVector outputVelocity = targetOffset.GetUnsafeNormal() * ProjectileVelocity;


				float angleBetween = FMath::Acos(FVector::DotProduct(targetOffset.GetUnsafeNormal() * -1.0f, pawnVelocity.GetUnsafeNormal()));

				if (!PredictTargetPosition && pawnVelocity.Size() != 0.0f && !(pawnVelocity.Size() > ProjectileVelocity && FMath::Sin(angleBetween) / ProjectileVelocity > FMath::Cos(angleBetween) / pawnVelocity.Size()))
				{
					float shootAngle = FMath::Asin(FMath::Sin(angleBetween) * pawnVelocity.Size() / ProjectileVelocity);
					outputVelocity = TargetPawn->GetActorLocation() + pawnVelocity * targetOffset.Size() / FMath::Sin(UKismetMathLibrary::GetPI() - angleBetween - shootAngle) * FMath::Sin(shootAngle) / pawnVelocity.Size();
					outputVelocity -= GetActorLocation() + MuzzleOffset;
					outputVelocity = outputVelocity.GetUnsafeNormal() * ProjectileVelocity;
				}

				projectile->SetVelocity(outputVelocity);

				CanShoot = false;
				GetWorldTimerManager().SetTimer(ShootCooldownTimer, this, &ATurretCharacter::EndCooldown, ShootingCooldown, false, ShootingCooldown);
			}
		}
	}
	return false;
}

void ATurretCharacter::EndCooldown()
{
	CanShoot = true;
	GetWorldTimerManager().ClearTimer(ShootCooldownTimer);
}
