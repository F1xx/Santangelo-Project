// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Effect.h"
#include "Components/BuffDebuffSystem.h"
#include "Characters/SAPlayer.h"
#include "Components/Health/HealthComponent.h"
#include "Components/Stamina/StaminaComponent.h"

UEffect::UEffect()
{
}

UEffect::UEffect(const UEffect* eff)
{
	IsTerminating = eff->IsTerminating;
	Type = eff->Type;
	TimeToEffect = eff->TimeToEffect;
	IsPermanent = eff->IsPermanent;
	EffectEndConditions = eff->EffectEndConditions;
}

float UEffect::GetPercentComplete()
{
	if (GetWorld()->GetTimerManager().IsTimerActive(EffectTimerHandle) == false)
	{
		return 0.0f;
	}

	float elapsed = GetWorld()->GetTimerManager().GetTimerElapsed(EffectTimerHandle);

	return elapsed / TimeToEffect;
}

void UEffect::AddEffect(UBuffDebuffSystem* owner)
{
	SetOwningSystem(owner);

	if (IsPermanent == false)
	{
		//sets itself to be deleted when the timer ends

		GetWorld()->GetTimerManager().SetTimer(EffectTimerHandle, this, &UEffect::QueueSelfForRemoval, TimeToEffect, false);
	}

	SetupExitConditions();
}

void UEffect::QueueSelfForRemoval()
{
	m_OwningSystem->QueueForRemoval(this);
}

void UEffect::RemoveEffect()
{
	RemoveDelegates();
	GetWorld()->GetTimerManager().ClearTimer(EffectTimerHandle);
}

void UEffect::SetupExitConditions()
{
	ASAPlayer* player = Cast<ASAPlayer>(m_OwningSystem->GetOwner());

	for (TEnumAsByte<EndConditions> eff : EffectEndConditions)
	{
		switch (eff)
		{
		case EndConditions::DAMAGETAKEN_EC:
			if (player)
			{
				player->GetHealthComponent()->OnDamageDelegate.AddDynamic(this, &UEffect::QueueSelfForRemoval);
			}
			break;
		case EndConditions::HEALING_EC:
			break;
		case EndConditions::OUTOFSTAMINA_EC:
			break;
		default:
			break;
		}
	}
}

void UEffect::RemoveDelegates()
{
	ASAPlayer* player = Cast<ASAPlayer>(m_OwningSystem->GetOwner());

	for (TEnumAsByte<EndConditions> eff : EffectEndConditions)
	{
		switch (eff)
		{
		case EndConditions::DAMAGETAKEN_EC:
			if (player)
			{
				player->GetHealthComponent()->OnDamageDelegate.RemoveDynamic(this, &UEffect::QueueSelfForRemoval);
			}
			break;
		case EndConditions::HEALING_EC:
			break;
		case EndConditions::OUTOFSTAMINA_EC:
			break;
		default:
			break;
		}
	}
}

class UWorld* UEffect::GetWorld() const
{
	return m_OwningSystem->GetWorld();
}