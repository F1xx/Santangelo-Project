// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "Effect.generated.h"

//note this is rather arbitrary as an effect could have both positive and negative effects
UENUM()
enum EffectType
{
	NONE_EFFECT     UMETA(DisplayName = "NONE"),
	BUFF_EFFECT     UMETA(DisplayName = "Buff"),
	DEBUFF_EFFECT   UMETA(DisplayName = "Debuff"),
};

//Enum to be used for reasons under which the effect will self terminate.
UENUM()
enum EndConditions
{
	HEALING_EC			UMETA(DisplayName = "Get Healed"),
	DAMAGETAKEN_EC		UMETA(DisplayName = "Take Damage"),
	OUTOFSTAMINA_EC		UMETA(DisplayName = "Out of Stamina"),
};

UCLASS(Blueprintable)
class SANTANGELO_API UEffect : public UObject
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEffect();
	UEffect(const UEffect* eff);

	//returns how much the effect has completed from 0 to 1
	UFUNCTION()
		float GetPercentComplete();

	FORCEINLINE void SetOwningSystem(class UBuffDebuffSystem* sys) { m_OwningSystem = sys; }

	UFUNCTION()
		void AddEffect(UBuffDebuffSystem* owner);
	UFUNCTION()
		void QueueSelfForRemoval();
	UFUNCTION()
		void RemoveEffect();

protected:
	void SetupExitConditions();
	void RemoveDelegates();

	virtual class UWorld* GetWorld() const override;


public:
#pragma region BASE UNIVERSAL SETTINGS
	//if this is true none of its effects will be applied
	bool IsTerminating = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BASESETTINGS)
		float TimeToEffect = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BASESETTINGS)
		TEnumAsByte<EffectType> Type = EffectType::NONE_EFFECT;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BASESETTINGS)
		bool IsPermanent = false;

	//if any of these conditions are met this effect will, as soon as possible, self terminate.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BASESETTINGS)
		TArray<TEnumAsByte<EndConditions>> EffectEndConditions;
#pragma endregion

#pragma region Health Settings

#pragma endregion

#pragma region Stamina Settings

#pragma endregion

#pragma region Movement Settings

#pragma endregion

protected:
	FTimerHandle EffectTimerHandle;

	class UBuffDebuffSystem* m_OwningSystem;
};