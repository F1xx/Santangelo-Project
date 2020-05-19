// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "SantangeloGameMode.h"
#include "SantangeloHUD.h"
#include "UObject/ConstructorHelpers.h"
#include "SAPlayer.h"

ASantangeloGameMode::ASantangeloGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/SantangeloProject/Blueprints/Characters/BP_SAPlayer"));
	//DefaultPawnClass = ASAPlayer::StaticClass();

	//// use our custom HUD class
	//HUDClass = ASantangeloHUD::StaticClass();
}
