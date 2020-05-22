// Fill out your copyright notice in the Description page of Project Settings.


#include "StaminaComponent.h"
#include "BaseEquipment.h"

// Sets default values for this component's properties
UStaminaComponent::UStaminaComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UStaminaComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


float UStaminaComponent::GetCostAfterWeight(float baseCost)
{
	return baseCost;
}

bool UStaminaComponent::CheckAndConsumeStamina(float cost)
{
	if (HasEnoughStamina(cost))
	{
		UseStamina(cost);
		return true;
	}
	else
	{
		return false;
	}
}

bool UStaminaComponent::CheckStamina(float cost)
{
	if (HasEnoughStamina(cost))
	{
		return true;
	}
	else
	{
		return false;
	}
}

//returns false if stamina has run out
bool UStaminaComponent::IsStaminaDepleted()
{
	if (CurrentStamina <= 0.0f)
	{
		return true;
	}

	return false;
}



// Called every frame
void UStaminaComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//consume stamina if sprinting
	if (m_IsSprinting)
	{
		CheckAndConsumeStamina(GetCostAfterWeight(BaseSprintCostPerSecond * DeltaTime));
		//GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Blue, FString::Printf(TEXT("Sprinting")));

	}

	if (IsStaminaDepleted())
	{
		//turn off any progressive stamina users (sprint for example)
		StopSprinting();
	}

	//regen stamina
	if (CurrentStamina < MaxStamina && GetWorld()->GetTimerManager().IsTimerActive(RegenDelayHandle) == false)
	{
		CurrentStamina = FMath::Clamp(CurrentStamina + GetCurrentRegenRate() * DeltaTime, 0.0f, MaxStamina);
	}
}

void UStaminaComponent::ChangeEquipment(UBaseEquipment* oldEquip, UBaseEquipment* newequip)
{
	if (oldEquip != nullptr)
	{
		RemoveWeight(oldEquip->GetWeight());
	}

	if (newequip != nullptr)
	{
		AddWeight(newequip->GetWeight());
	}
}

void UStaminaComponent::UseStamina(float amount)
{
	CurrentStamina -= amount;
	GetWorld()->GetTimerManager().SetTimer(RegenDelayHandle, nullptr, RegenDelay, false);
}

float UStaminaComponent::GetCurrentRegenRate()
{
	return BaseRegenRate;
}

bool UStaminaComponent::Jump()
{
	return CheckAndConsumeStamina(GetCostAfterWeight(BaseJumpCost));
}

bool UStaminaComponent::StartSprinting()
{
	return false;
}

bool UStaminaComponent::Dodge()
{
	return CheckAndConsumeStamina(GetCostAfterWeight(BaseDodgeCost));
}

void UStaminaComponent::Sprint()
{
	if (CheckStamina(GetCostAfterWeight(BaseDodgeCost)))
	{
		m_IsSprinting = true;
	}
}

void UStaminaComponent::StopSprinting()
{
	if (m_IsSprinting)
	{
		m_IsSprinting = false;
	}
}

bool UStaminaComponent::HasEnoughStamina(float amountNeeded)
{
	if (CurrentStamina - amountNeeded < 0.0f)
	{
		return false;
	}
	return true;
}
