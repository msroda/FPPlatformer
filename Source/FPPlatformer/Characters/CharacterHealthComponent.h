// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "CharacterHealthComponent.generated.h"

UENUM()	
enum class EDamageType
{
	DMG_Physical,
	DMG_Explosive,
	DMG_Ice,
	DMG_Fire,
	DMG_Energy,
	DMG_MAX
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FKillCharacter);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDamageCharacter, float, damage, EDamageType, damageType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOvertimeEventEnded, EDamageType, damageType);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FPPLATFORMER_API UCharacterHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCharacterHealthComponent();

	// Character's max HP
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Health)
		float MaxHealth = 100.0f;

	// Character's current HP
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Health)
		float CurrentHealth;

	// Character's HP regen rate
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Health)
		float RegenerationRate = 0.0f;

	// Character's HP regen cooldown
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Health)
		float RegenerationCooldown = 10.0f;

	// Character's resistancies to physical/explosive/ice/fire/energy dmg
	UPROPERTY(EditAnywhere, EditFixedSize, BlueprintReadWrite, Category = Resistance)
		TArray<float> Resistances;

	UPROPERTY(EditAnywhere, EditFixedSize, BlueprintReadWrite, Category = EventsOverTime)
		TArray<int> EventsDuration;

	UPROPERTY(EditAnywhere, EditFixedSize, BlueprintReadWrite, Category = EventsOverTime)
		TArray<float> EventsDamageOnTick;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EventsOverTime)
		float TicksPerSecond;

	// Event to broadcast when health reaches 0
	UPROPERTY()
		FKillCharacter KillCharacter;

	// Event to broadcast when actor is damaged
	UPROPERTY()
		FDamageCharacter DamageCharacter;

	// Event to broadcast when overtime effect has ended
	UPROPERTY()
		FOvertimeEventEnded OvertimeEventEnded;

	UFUNCTION()
		void OvertimeEventsTick();
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Handle input damage
	void Damage(EDamageType dmgType, float value);

	// For clearing timer when bullet is destroyed
	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;

	// For clearing timer when game stops
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void Rebirth();

	void Kill();

private:

	// Timer for hp regeneration cooldown
	float RegenTimer;

	TArray<int> CurrentTicks;

	FTimerHandle OvertimeEventsTicker;
};
