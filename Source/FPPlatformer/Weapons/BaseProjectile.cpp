// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseProjectile.h"

// Sets default values
ABaseProjectile::ABaseProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));

	velocitySet = true;
}

// Called when the game starts or when spawned
void ABaseProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	ProjectilePrimitive = FindComponentByClass<UPrimitiveComponent>();
	if (ProjectilePrimitive)
	{
		ProjectilePrimitive->OnComponentHit.AddDynamic(this, &ABaseProjectile::OnHit);
		ProjectilePrimitive->OnComponentBeginOverlap.AddDynamic(this, &ABaseProjectile::OnOverlapBegin);
	}

}

// Called every frame
void ABaseProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!velocitySet)
	{
		//velocitySet = true;
		ProjectileMovement->Velocity = desiredVelocity;
	}

}


void ABaseProjectile::SetVelocity(FVector newVelocity)
{
	desiredVelocity = newVelocity;
	velocitySet = false;
}

void ABaseProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
        	UCharacterHealthComponent* HPComp = OtherActor->FindComponentByClass<UCharacterHealthComponent>();

	if (HPComp)
	{
		HPComp->Damage(DamageType, Damage);
	}

	Destroy();
}

void ABaseProjectile::OnOverlapBegin(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (CanGoThroughWater && OtherActor->ActorHasTag("Water"))
		return;

	UCharacterHealthComponent* HPComp = OtherActor->FindComponentByClass<UCharacterHealthComponent>();

	if (HPComp)
	{
		HPComp->Damage(DamageType, Damage);
	}

	Destroy();
}
