// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/BaseProjectile.h"
#include "Weapons/Explosion.h"
#include "StickyBombProjectile.generated.h"

/**
 * 
 */
UCLASS()
class FPPLATFORMER_API AStickyBombProjectile : public ABaseProjectile
{
	GENERATED_BODY()
	
public:
	   
	AStickyBombProjectile();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AExplosion> Explosion;


	virtual void OnHit(UPrimitiveComponent * HitComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, FVector NormalImpulse, const FHitResult & Hit) override;
	
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult) override;

	void Explode();
};
