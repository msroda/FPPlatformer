// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SwitchableComponent.h"
#include "MovingObject.generated.h"

UCLASS()
class FPPLATFORMER_API AMovingObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMovingObject();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		AActor* Target;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		float MovingTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		USwitchableComponent* SwitchableComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void Switch();

private:

	FVector StartPosition;

	FVector TargetPosition;

	float DeltaPosition;

	bool Direction;
};
