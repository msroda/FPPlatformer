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
			TArray<FHitResult> outHits;
			bool isHit = false;

			TArray<AActor*> actorsToIgnore;

			isHit = UKismetSystemLibrary::SphereTraceMulti(this, SphereComponent->GetComponentLocation(), SphereComponent->GetComponentLocation(), Radius * 50, UEngineTypes::ConvertToTraceType(ECC_Visibility), false, actorsToIgnore, EDrawDebugTrace::Persistent, outHits, true, FLinearColor::Red, FLinearColor::Green, 0.0f);

			if (isHit)
			{
				TArray<AActor*> hitActors;
				for (auto outHit : outHits)
				{
					AActor* tempActor = outHit.Actor.Get();
					if (!hitActors.Contains(tempActor))
					{
						hitActors.Add(tempActor);

						UCharacterHealthComponent* otherHP = Cast<UCharacterHealthComponent>(tempActor->GetComponentByClass(UCharacterHealthComponent::StaticClass()));
						if (otherHP)
						{
							otherHP->Damage(DamageType, Damage);
						}
					}
				}
			}

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

