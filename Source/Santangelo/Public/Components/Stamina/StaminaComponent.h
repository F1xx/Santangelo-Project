// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Effects/EffectableComponent.h"
#include "StaminaComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStaminaDrainedDelegate);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SANTANGELO_API UStaminaComponent : public UEffectableComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStaminaComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//Removes the old weight and adds the new. Useful for easily changing gear if the player swaps out a gear slot. Nullptrs are handled
	void ChangeEquipment(class UBaseEquipment* oldEquip, class UBaseEquipment* newequip);

	FORCEINLINE void AddWeight(float amount) { CurrentWeight += amount; /*event on overencumberance?*/}
	FORCEINLINE void RemoveWeight(float amount) { CurrentWeight -= amount; }
	FORCEINLINE float GetCurrentWeight() { return CurrentWeight; }
	FORCEINLINE float GetCurrentWeightPercentage() { return CurrentWeight / MaxWeight; }

	UFUNCTION(BlueprintCallable)
		FORCEINLINE float GetCurrentStamina() { return CurrentStamina; }
	UFUNCTION(BlueprintCallable)
		FORCEINLINE float GetStaminaPercent() { return CurrentStamina / MaxStamina; }

	//uses the stamina whether it has enough or not. Returns true if it had enough, false if not.
	//This means if a jump costs 15 and you have 10 you will still jump but false will be returned
	//Broadcasts the drained event if out of stamina
	bool UseStamina(float amount);

	float GetCurrentRegenRate();

	bool Jump();
	bool StartSprinting();
	bool Dodge();

	void Sprint();
	void StopSprinting();

	UFUNCTION(BlueprintCallable)
		FORCEINLINE bool IsSprinting() { return m_IsSprinting; }

	//returns true if there is enough stamina to do what is needed.
	bool HasEnoughStamina(float amountNeeded);

	bool IsStaminaDepleted();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	float GetCostAfterWeight(float baseCost);

	bool CheckAndConsumeStamina(float cost);

public:
	UPROPERTY(BlueprintAssignable, Category = "Events")
		FStaminaDrainedDelegate OnStaminaDrainedDelegate;

protected:
	float MaxStamina = 100.0f;
	float CurrentStamina = 100.0f;

	float MaxWeight = 100.0f;
	float CurrentWeight = 0.0f;

	bool m_IsSprinting = false;

	FTimerHandle RegenDelayHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stamina)
		float RegenDelay = 1.0f; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stamina)
		float BaseRegenRate = 3.0f; //this will be affected by weight

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stamina)
		float BaseJumpCost = 15.0f; //this will be affected by weight

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stamina)
		float BaseDodgeCost = 20.0f; //this will be affected by weight

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stamina)
		float BaseSprintCostPerSecond = 5.0f; //this will be affected by weight
};
