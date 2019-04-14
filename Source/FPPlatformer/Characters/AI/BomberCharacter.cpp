// Fill out your copyright notice in the Description page of Project Settings.

#include "BomberCharacter.h"
#include "Runtime/Engine/Classes/GameFramework/Controller.h"
#include "Engine/Classes/Particles/ParticleSystemComponent.h"
#include "Engine/Classes/Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Runtime/Engine/Public/TimerManager.h"

// Sets default values
ABomberCharacter::ABomberCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CharacterHealth = CreateDefaultSubobject<UCharacterHealthComponent>(TEXT("CharacterHealth"));
	CharacterHealth->MaxHealth = 100;

	CharacterHealth->KillCharacter.AddDynamic(this, &ABomberCharacter::Die);
	CharacterHealth->DamageCharacter.AddDynamic(this, &ABomberCharacter::ReceiveDamage);
	CharacterHealth->OvertimeEventEnded.AddDynamic(this, &ABomberCharacter::OnDamageEventEnded);

	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("CharacterSensing"));
	PawnSensingComponent->OnSeePawn.AddDynamic(this, &ABomberCharacter::OnSeePlayer);

	OriginalWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
}

// Called when the game starts or when spawned
void ABomberCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (FireParticleTemplate)
	{
		FireParticleSystem = UGameplayStatics::SpawnEmitterAttached(FireParticleTemplate, RootComponent, NAME_None, GetActorLocation(), GetActorRotation(), EAttachLocation::KeepWorldPosition, false);
		FireParticleSystem->DeactivateSystem();
	}
}

// Called every frame
void ABomberCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsAlerted)
	{
		AlertTimer += DeltaTime;

		if (AlertTimer > PlayerLoseTime)
			CanSeePlayer = false;

		if (AlertTimer > AlertTime)
			IsAlerted = false;;
	}

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
				if (Direction)
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

void ABomberCharacter::Explode()
{
	FActorSpawnParameters SpawnParams;

	GetWorld()->SpawnActor<AExplosion>(Explosion, GetActorLocation(), GetActorRotation(), SpawnParams);
	Destroy();
}

void ABomberCharacter::Die()
{
	if (!IsExploding)
	{
		GetWorldTimerManager().SetTimer(ExplosionCountdownTimer, this, &ABomberCharacter::Explode, FuseTime, false, FuseTime);
		Controller->StopMovement();
		IsExploding = true;
		if (SmokeParticleTemplate)
		{
			SmokeParticleSystem = UGameplayStatics::SpawnEmitterAttached(SmokeParticleTemplate, RootComponent, NAME_None, GetActorLocation(), GetActorRotation(), EAttachLocation::KeepWorldPosition, false);
			SmokeParticleSystem->CustomTimeDilation = 15.0f;
		}
	}
}

bool ABomberCharacter::Shoot()
{
	return false;
}

void ABomberCharacter::Destroyed()
{
	Super::Destroyed();
	GetWorldTimerManager().ClearTimer(ExplosionCountdownTimer);
}

void ABomberCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	GetWorldTimerManager().ClearTimer(ExplosionCountdownTimer);
}

void ABomberCharacter::ReceiveDamage(float damage, EDamageType damageType)
{
	if (damageType == EDamageType::DMG_Fire)
	{		
		if (FireParticleSystem)
		{
			FireParticleSystem->ActivateSystem();
		}
	}

	else if (damageType == EDamageType::DMG_Ice)
	{
		GetCharacterMovement()->MaxWalkSpeed = OriginalWalkSpeed * SlowMultiplier;
	}
}

void ABomberCharacter::OnDamageEventEnded(EDamageType damageType)
{
	if (damageType == EDamageType::DMG_Fire)
	{
		if (FireParticleSystem)
		{
			FireParticleSystem->DeactivateSystem();
		}
	}

	else if (damageType == EDamageType::DMG_Ice)
	{
		GetCharacterMovement()->MaxWalkSpeed = OriginalWalkSpeed;
	}
}

void ABomberCharacter::OnSeePlayer(APawn* Pawn)
{
	IsAlerted = true;
	CanSeePlayer = true;
	AlertTimer = 0.0f;
	LastPlayersLocation = Pawn->GetActorLocation();
	TargetPawn = Pawn;
}
