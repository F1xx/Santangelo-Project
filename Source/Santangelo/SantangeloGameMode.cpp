// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "SantangeloGameMode.h"
#include "SantangeloHUD.h"
#include "SantangeloCharacter.h"
#include "UObject/ConstructorHelpers.h"

ASantangeloGameMode::ASantangeloGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ASantangeloHUD::StaticClass();
}
