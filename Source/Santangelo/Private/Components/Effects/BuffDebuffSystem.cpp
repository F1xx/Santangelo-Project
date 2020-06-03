// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Effects/BuffDebuffSystem.h"
#include "Components/Effects/Effect.h"
#include "Components/Effects/StaminaEffects.h"

// Sets default values for this component's properties
UBuffDebuffSystem::UBuffDebuffSystem()
{
	PrimaryComponentTick.bCanEverTick = true;
}


UBuffDebuffSystem::~UBuffDebuffSystem()
{
	m_EffectsToRemove.Empty();
	m_EffectsToAdd.Empty();
	m_Effects.Empty();
}

// Called when the game starts
void UBuffDebuffSystem::BeginPlay()
{
	Super::BeginPlay();
}

void UBuffDebuffSystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//handle all effects
	//GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Blue, FString::Printf(TEXT("Effects: %d"), m_Effects.Num()));
	//GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Blue, FString::Printf(TEXT("To Remove: %d"), m_EffectsToRemove.Num()));

	//Handle memory management
	HandleMemoryManagement();
}

#pragma region Memory Management

void UBuffDebuffSystem::HandleMemoryManagement()
{
	//if there are any effects to remove, remove them
	for (UEffect* eff : m_EffectsToRemove)
	{
		RemoveEffect(eff);
	}
	m_EffectsToRemove.Empty();

	//if there are any effects to add, add them
	for (UEffect* eff : m_EffectsToAdd)
	{
		m_Effects.AddUnique(eff);
	}
	m_EffectsToAdd.Empty();
}


void UBuffDebuffSystem::RemoveEffect(UEffect* effect)
{
	if (effect != nullptr)
	{
		int index = m_Effects.Find(effect);

		if (index != INDEX_NONE)
		{
			m_Effects[index]->RemoveEffect();

			//by default the shrink value is true so using removeat like this will trim the array
			m_Effects.RemoveAt(index);
		}
	}
}

void UBuffDebuffSystem::QueueForRemoval(UEffect* effect)
{
	if (effect != nullptr)
	{
		m_EffectsToRemove.AddUnique(effect);
	}
}

void UBuffDebuffSystem::QueueForAddition(UEffect* effect)
{
	if (effect != nullptr)
	{
		//Check if it actually gets applied based on its chance
		float randomnum = FMath::FRandRange(0.0001f, 1.0f);

		float chance = effect->RandomChance ? FMath::FRandRange(0.0f, 1.0f) : effect->ChanceOfEffecting;

		if (randomnum > chance)
		{
			//don't add it if the chance didn't work out
			return;
		}

		int index = m_EffectsToAdd.AddUnique(effect);

		if (index != INDEX_NONE)
		{
			effect->AddEffect(this);
		}
	}
}

//queues every effect currently on the character for removal
void UBuffDebuffSystem::ClearEffects()
{
	for (UEffect* eff : m_Effects)
	{
		QueueForRemoval(eff);
	}
}
#pragma endregion