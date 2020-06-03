// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Effects/EffectableComponent.h"
#include "Components/Effects/BaseEffectStruct.h"
#include "Components/Effects/BuffDebuffSystem.h"
#include "Characters/SAPlayer.h"

// Sets default values for this component's properties
UEffectableComponent::UEffectableComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

UEffectableComponent::~UEffectableComponent()
{
	if (EffectsStruct)
	{
		delete EffectsStruct;
		EffectsStruct = nullptr;
	}
}

// Called when the game starts
void UEffectableComponent::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(EffectsTickHandle, this, &UEffectableComponent::UpdateEffects, 0.5f, true);
}

void UEffectableComponent::UpdateEffects()
{
	UBuffDebuffSystem* sys = GetEffectsSystem();

	if (sys == nullptr || EffectsStruct == nullptr)
	{
		return;
	}

	EffectsStruct->UpdateStruct(sys->GetAllEffects());
}

//this can return nullptr
ASAPlayer* UEffectableComponent::GetCharacterOwner()
{
	return Cast<ASAPlayer>(GetOwner());
}

UBuffDebuffSystem* UEffectableComponent::GetEffectsSystem()
{
	ASAPlayer* player = GetCharacterOwner();

	if (player)
	{
		return player->GetEffectSystemComponent();
	}
	return nullptr;
}