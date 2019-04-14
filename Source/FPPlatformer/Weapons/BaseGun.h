// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/Classes/Camera/CameraShake.h"
#include "Engine/Classes/GameFramework/PlayerController.h"
#include "BaseGun.generated.h"

UCLASS()
class FPPLATFORMER_API ABaseGun : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseGun();

	// Muzzle position
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gun)
	FVector MuzzleOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gun)
		float AimingSpeed;

	virtual void SetActive(bool active);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;	

	// Spawn and shoot projectile
	AActor* ShootProjectile(TSubclassOf<class AActor> projectileClass, float speed);

	// Spawn and shoot projectile
	bool ShootHitscan(FVector target, FHitResult &outHit);

	FVector LookedTarget;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Primary fire start
	virtual void OnFirePressed();

	// Primary fire stop
	virtual void OnFireReleased();

	// Secondary fire start
	virtual void OnAltFirePressed();

	// Secondary fire stop
	virtual void OnAltFireReleased();

	void Aim(FVector target, float delta);

	bool IsActive;
};
