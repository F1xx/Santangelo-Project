// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Effects//Effect.h"

/**
 * 
 */
struct SANTANGELO_API BaseEffectStruct
{
    virtual void UpdateStruct(TArray<UEffect*> effects) = 0;
};
