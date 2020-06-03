// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Effects/BaseEffectStruct.h"

/**
 * 
 */
struct SANTANGELO_API HealthEffects : BaseEffectStruct
{
	float HealthRegenPerSecond = 0.0f;
	float HealthDamagePerSecond = 0.0f;

	void UpdateStruct(TArray<UEffect*> effects) override
	{
		HealthDamagePerSecond = 0.0f;
		HealthRegenPerSecond = 0.0f;

		for (UEffect* eff : effects)
		{
			HealthDamagePerSecond += eff->HealthDamagePerSecond;
			HealthRegenPerSecond += eff->HealthRegenPerSecond;
		}
	}
};
