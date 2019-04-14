// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/GameFramework/ProjectileMovementComponent.h"
#include "Characters/CharacterHealthComponent.h"
#include "BaseProjectile.generated.h"

UCLASS()
class FPPLATFORMER_API ABaseProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseProjectile();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UPrimitiveComponent* ProjectilePrimitive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile)
		float Damage = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile)
		EDamageType DamageType = EDamageType::DMG_Physical;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile)
		bool CanGoThroughWater;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetVelocity(FVector newVelocity);

	UFUNCTION()
		virtual void OnHit(UPrimitiveComponent * HitComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, FVector NormalImpulse, const FHitResult & Hit);

	UFUNCTION()
		virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	AActor* Shooter;

private:
	bool velocitySet;

	FVector desiredVelocity;
};
