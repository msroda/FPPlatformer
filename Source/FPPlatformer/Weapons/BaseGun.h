// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseGun.generated.h"

UCLASS()
class FPPLATFORMER_API ABaseGun : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseGun();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;	

	/** Spawn and shoot projectile */
	void ShootProjectile(FVector target, TSubclassOf<class AActor> projectileClass, float speed);

	/** Spawn and shoot projectile */
	bool ShootHitscan(FVector target, FHitResult &outHit);

	/** Muzzle position */
	FVector MuzzleOffset;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Primary fire start */
	virtual void OnFirePressed(FVector target);

	/** Primary fire stop */
	virtual void OnFireReleased(FVector target);

	/** Secondary fire start */
	virtual void OnAltFirePressed(FVector target);

	/** Secondary fire stop */
	virtual void OnAltFireReleased(FVector target);

};
