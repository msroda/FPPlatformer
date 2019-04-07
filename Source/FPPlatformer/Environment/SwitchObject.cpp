// Fill out your copyright notice in the Description page of Project Settings.

#include "SwitchObject.h"
#include "SwitchableComponent.h"

// Sets default values
ASwitchObject::ASwitchObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	HealthComponent = CreateDefaultSubobject<UCharacterHealthComponent>(TEXT("CharacterHealth"));

	HealthComponent->DamageCharacter.AddDynamic(this, &ASwitchObject::ReceiveDamage);
}

void ASwitchObject::ReceiveDamage(float damage, EDamageType damageType)
{
	SwitchAll();
}

// Called when the game starts or when spawned
void ASwitchObject::BeginPlay()
{
	Super::BeginPlay();
	
	IsOnCooldown = false;
}

// Called every frame
void ASwitchObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASwitchObject::SwitchAll()
{
	if (!IsOnCooldown)
	{
		for (auto object : ObjectsToSwitch)
		{
			USwitchableComponent* switchableComponent = object->FindComponentByClass<USwitchableComponent>();
			if (switchableComponent)
			{
				switchableComponent->Switch();
			}
		}
		GetWorldTimerManager().SetTimer(CooldownTimerHandle, this, &ASwitchObject::EndCooldown, Cooldown, true, Cooldown);
		IsOnCooldown = true;
	}
}

void ASwitchObject::EndCooldown()
{
	IsOnCooldown = false;
	GetWorldTimerManager().ClearTimer(CooldownTimerHandle);
}

void ASwitchObject::Destroyed()
{
	Super::Destroyed();
	GetWorldTimerManager().ClearTimer(CooldownTimerHandle);
}

void ASwitchObject::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	GetWorldTimerManager().ClearTimer(CooldownTimerHandle);
}
