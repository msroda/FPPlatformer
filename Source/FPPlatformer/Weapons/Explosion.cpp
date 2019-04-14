// Fill out your copyright notice in the Description page of Project Settings.

#include "Explosion.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"

// Sets default values
AExplosion::AExplosion()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//SphereComponent->CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	//SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ExplosionTimer = 0.0f;
}

// Called when the game starts or when spawned
void AExplosion::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(SphereTraceTimer, this, &AExplosion::Trace, DurationTime / Ticks, true, DurationTime / Ticks);
}

// Called every frame
void AExplosion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(SphereComponent)
	{
		if (ExplosionTimer < DurationTime)
		{
			ExplosionTimer += DeltaTime;
			SphereComponent->SetRelativeScale3D(FVector::OneVector * (ExplosionTimer / DurationTime) * Radius);
		}
		else
		{
			

			Destroy();
		}
	}
}

void AExplosion::OnConstruction(const FTransform & Transform)
{
	SphereComponent = FindComponentByClass<UPrimitiveComponent>();
	if (SphereComponent)
	{
		SphereComponent->SetRelativeScale3D(FVector::ZeroVector);
		SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AExplosion::Trace()
{
	TArray<FHitResult> outHits;
	bool isHit = false;

	TArray<AActor*> actorsToIgnore;

	isHit = UKismetSystemLibrary::SphereTraceMulti(this, SphereComponent->GetComponentLocation(), SphereComponent->GetComponentLocation(), Radius * 50, UEngineTypes::ConvertToTraceType(ECC_Visibility), false, actorsToIgnore, EDrawDebugTrace::Persistent, outHits, true, FLinearColor::Red, FLinearColor::Green, 0.0f);

	if (isHit)
	{
		for (auto outHit : outHits)
		{
			AActor* tempActor = outHit.Actor.Get();
			if (!ActorsHit.Contains(tempActor))
			{
				FHitResult outSingleHit;
				bool isSingleHit = UKismetSystemLibrary::LineTraceSingle(this, SphereComponent->GetComponentLocation(), outHit.ImpactPoint, UEngineTypes::ConvertToTraceType(ECC_Visibility), false, actorsToIgnore, EDrawDebugTrace::Persistent, outSingleHit, true, FLinearColor::Red, FLinearColor::Green, 0.0f);

				if (!isSingleHit || outSingleHit.Actor.Get() == tempActor)
				{
					ActorsHit.Add(tempActor);

					UCharacterHealthComponent* otherHP = Cast<UCharacterHealthComponent>(tempActor->GetComponentByClass(UCharacterHealthComponent::StaticClass()));
					if (otherHP)
					{
						float dmg;

						if (outSingleHit.Distance / 50.0f > FallofStartDistance && FallofStartDistance < Radius)
							dmg = (1 - (outSingleHit.Distance / 50.0f - FallofStartDistance) / (Radius - FallofStartDistance)) * Damage;
						else
							dmg = Damage;
						
						otherHP->Damage(DamageType, dmg);
					}
				}				
			}
		}
	}
}

void AExplosion::Destroyed()
{
	GetWorldTimerManager().ClearTimer(SphereTraceTimer);
	Super::Destroyed();
}

void AExplosion::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetWorldTimerManager().ClearTimer(SphereTraceTimer);
	Super::EndPlay(EndPlayReason);
}
