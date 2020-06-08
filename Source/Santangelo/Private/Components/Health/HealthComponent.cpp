// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Health/HealthComponent.h"
#include "GameFramework/Actor.h"
#include "Components/Effects/HealthEffects.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	CurrentHealth = MaxHealth;
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	EffectsStruct = new HealthEffects();

	AActor* Owner = GetOwner();

	if (Owner)
	{
		Owner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::TakeDamage);
	}
}

void UHealthComponent::OnDeath()
{
	//make an on death event
	OnDeathDelegate.Broadcast();
}

void UHealthComponent::CheckHealth()
{
	if (!IsAlive())
	{
		OnDeath();
	}
}

// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (((HealthEffects*)EffectsStruct)->HealthDamagePerSecond != 0.0f)
	{
		TakeDamage(GetOwner(), ((HealthEffects*)EffectsStruct)->HealthDamagePerSecond * DeltaTime, nullptr, nullptr, nullptr);
	}

	if (GetWorld()->GetTimerManager().IsTimerActive(RegenDelayHandle) == false && CurrentHealth < MaxHealth && CanRegen)
	{
		//innate healing
		HealNoEvent(RegenPerSecond * DeltaTime);

		//healing buffs/effects
		if (((HealthEffects*)EffectsStruct)->HealthRegenPerSecond != 0.0f)
		{
			HealNoEvent(((HealthEffects*)EffectsStruct)->HealthRegenPerSecond * DeltaTime);
		}
	}
}

void UHealthComponent::TakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0 || ((HealthEffects*)EffectsStruct)->IsInvulnerable)
	{
		return;
	}

	//make this more complicated when its actually used
	CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.f, MaxHealth);
	OnDamageDelegate.Broadcast();

	GetWorld()->GetTimerManager().SetTimer(RegenDelayHandle, nullptr, RegenDelay, false);

	CheckHealth();
}

void UHealthComponent::Heal(float amount)
{
	CurrentHealth = FMath::Clamp(CurrentHealth + amount, 0.0f, MaxHealth);
	OnHealDelegate.Broadcast();
}

void UHealthComponent::HealNoEvent(float amount)
{
	CurrentHealth = FMath::Clamp(CurrentHealth + amount, 0.0f, MaxHealth);
}
