// Fill out your copyright notice in the Description page of Project Settings.

#include "FPPlatformerGameModeBase.h"
#include "Characters/Player/MyCharacter.h"
#include "UObject/ConstructorHelpers.h"

AFPPlatformerGameModeBase::AFPPlatformerGameModeBase()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FPCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;
}
