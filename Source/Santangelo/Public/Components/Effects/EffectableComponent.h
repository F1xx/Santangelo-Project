// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EffectableComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SANTANGELO_API UEffectableComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEffectableComponent();
	virtual ~UEffectableComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void UpdateEffects();
	class ASAPlayer* GetCharacterOwner();
	class UBuffDebuffSystem* GetEffectsSystem();

protected:
	FTimerHandle EffectsTickHandle;
	struct BaseEffectStruct* EffectsStruct;

};
