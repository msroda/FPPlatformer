// Fill out your copyright notice in the Description page of Project Settings.

#include "StickyBombProjectile.h"

AStickyBombProjectile::AStickyBombProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AStickyBombProjectile::OnHit(UPrimitiveComponent * HitComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, FVector NormalImpulse, const FHitResult & Hit)
{
	ProjectilePrimitive->SetSimulatePhysics(false);
	ProjectilePrimitive->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	ProjectileMovement->StopMovementImmediately();
	ProjectileMovement->Deactivate();
	
	AttachToActor(OtherActor, FAttachmentTransformRules::KeepWorldTransform);
	SetOwner(OtherActor);
}

void AStickyBombProjectile::OnOverlapBegin(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{

}

void AStickyBombProjectile::Explode()
{
	FActorSpawnParameters SpawnParams;

	GetWorld()->SpawnActor<AExplosion>(Explosion, GetActorLocation(), GetActorRotation(), SpawnParams);
	Destroy();
}
