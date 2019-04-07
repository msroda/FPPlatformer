// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Characters/CharacterHealthComponent.h"
#include "Engine/Public/TimerManager.h"
#include "SwitchObject.generated.h"

UCLASS()
class FPPLATFORMER_API ASwitchObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASwitchObject();

	UCharacterHealthComponent* HealthComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		TArray<AActor*> ObjectsToSwitch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		float Cooldown = 1.0f;

	UFUNCTION()
		void ReceiveDamage(float damage, EDamageType damageType);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void EndCooldown();

	FTimerHandle CooldownTimerHandle;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	/** For clearing timers when object is destroyed */
	virtual void Destroyed() override;

	/** For clearing timers when game stops */
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	bool IsOnCooldown;

	void SwitchAll();

};
