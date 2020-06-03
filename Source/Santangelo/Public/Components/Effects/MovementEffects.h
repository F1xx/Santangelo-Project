// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Effects/BaseEffectStruct.h"

/**
 * 
 */
struct SANTANGELO_API MovementEffects : BaseEffectStruct
{
	float MoveSpeedMod = 0.0f;

	void UpdateStruct(TArray<UEffect*> effects) override
	{
		MoveSpeedMod = 0.0f;

		for (UEffect* eff : effects)
		{
			MoveSpeedMod += eff->MoveSpeedMod;
		}
	}
};
