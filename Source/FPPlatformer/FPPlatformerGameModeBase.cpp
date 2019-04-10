// Fill out your copyright notice in the Description page of Project Settings.

#include "FPPlatformerGameModeBase.h"
#include "Characters/Player/MyCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/Classes/GameFramework/HUD.h"

AFPPlatformerGameModeBase::AFPPlatformerGameModeBase()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Characters/Player/FPCharacter"));
	static ConstructorHelpers::FClassFinder<AHUD> HudClassFinder(TEXT("/Game/MyHUD"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;
	HUDClass = HudClassFinder.Class;
}
