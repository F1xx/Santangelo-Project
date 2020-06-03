// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Effects/BaseEffectStruct.h"

/**
 * 
 */
struct SANTANGELO_API StaminaEffects : BaseEffectStruct
{
	float StaminaRegenPerSecond = 0.0f;
	float StaminaDamagePerSecond = 0.0f;

	void UpdateStruct(TArray<UEffect*> effects) override
	{
		StaminaDamagePerSecond = 0.0f;
		StaminaRegenPerSecond = 0.0f;

		for (UEffect* eff : effects)
		{
			StaminaDamagePerSecond += eff->StaminaDamagePerSecond;
			StaminaRegenPerSecond += eff->StaminaRegenPerSecond;
		}
	}
};
