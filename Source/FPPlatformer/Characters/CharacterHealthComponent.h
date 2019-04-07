// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterHealthComponent.generated.h"

UENUM()	
enum class EDamageType
{
	DMG_Physical,
	DMG_Explosive,
	DMG_Ice,
	DMG_Fire,
	DMG_Energy
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FKillCharacter);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDamageCharacter, float, damage, EDamageType, damageType);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FPPLATFORMER_API UCharacterHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCharacterHealthComponent();

	/** Character's max HP */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Health)
		float MaxHealth = 100.0f;

	/** Character's HP regen rate */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Health)
		float RegenerationRate = 0.0f;

	/** Character's HP regen cooldown */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Health)
		float RegenerationCooldown = 10.0f;

	/** Character's physical dmg resistance */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Resistance)
		float PhysicalResistance = 0.0f;

	/** Character's explosive dmg resistance */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Resistance)
		float ExplosiveResistance = 0.0f;

	/** Character's fire dmg resistance */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Resistance)
		float FireResistance = 0.0f;

	/** Character's ice dmg resistance */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Resistance)
		float IceResistance = 0.0f;

	/** Character's energy dmg resistance */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Resistance)
		float EnergyResistance = 0.0f;

	UPROPERTY()
		FKillCharacter KillCharacter;

	UPROPERTY()
		FDamageCharacter DamageCharacter;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/** Character's ice dmg resistance */
	void Damage(EDamageType dmgType, float value);

private:

	/** Character's current HP */
	float CurrentHealth;
};
