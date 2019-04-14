// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Characters/CharacterHealthComponent.h"
#include "Runtime/Engine/Classes/Materials/Material.h"
#include "FreezableWater.generated.h"

UCLASS()
class FPPLATFORMER_API AFreezableWater : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFreezableWater();

	UPrimitiveComponent* PrimitiveComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = World)
		bool IsIce;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = World)
		bool IsActive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = World)
		AFreezableWater* PairActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HealthAndDamage)
		UCharacterHealthComponent* HealthComponent;

	UFUNCTION()
		void ReceiveDamage(float damage, EDamageType damageType);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SetStateToIce();

	void SetStateToWater();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetActorEnabled(bool enabled);
};
