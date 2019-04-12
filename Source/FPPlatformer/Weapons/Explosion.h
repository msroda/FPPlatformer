// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Characters/CharacterHealthComponent.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "Runtime/Engine/Classes/Components/SphereComponent.h"
#include "Explosion.generated.h"

UCLASS()
class FPPLATFORMER_API AExplosion : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AExplosion();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called every frame
	virtual void OnConstruction(const FTransform & Transform) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Explosion)
		UPrimitiveComponent* SphereComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Explosion)
		float DurationTime = 0.3f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Explosion)
		float Radius = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Explosion)
		float Damage = 70.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Explosion)
		EDamageType DamageType = EDamageType::DMG_Explosive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Explosion)
		int Ticks = 4;

	UFUNCTION()
		void Trace();

private:
	TArray<AActor*> ActorsHit;

	FTimerHandle SphereTraceTimer;

	float ExplosionTimer;
};
