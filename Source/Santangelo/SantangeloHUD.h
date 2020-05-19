// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SantangeloHUD.generated.h"

UCLASS()
class ASantangeloHUD : public AHUD
{
	GENERATED_BODY()

public:
	ASantangeloHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

};

