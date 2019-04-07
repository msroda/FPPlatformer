// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SwitchableComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSwitchObjectState);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FPPLATFORMER_API USwitchableComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USwitchableComponent();

	UPROPERTY()
		FSwitchObjectState SwitchObjectState;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void Switch();
};
