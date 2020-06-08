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
	DISPELLED_EC		UMETA(DisplayName = "Dispelled"),
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
	UFUNCTION(BlueprintCallable)
		float GetPercentComplete();

	UFUNCTION(BlueprintCallable)
		float GetTimeRemaining();

	FORCEINLINE void SetOwningSystem(class UBuffDebuffSystem* sys) { m_OwningSystem = sys; }

	UFUNCTION(BlueprintCallable)
		void AddEffect(UBuffDebuffSystem* owner);
	UFUNCTION(BlueprintCallable)
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
		class UTexture2D* EffectSprite;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BASESETTINGS)
		float TimeToEffect = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BASESETTINGS, meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
		float ChanceOfEffecting = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BASESETTINGS)
		bool RandomChance = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BASESETTINGS)
		TEnumAsByte<EffectType> Type = EffectType::NONE_EFFECT;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BASESETTINGS)
		bool IsPermanent = false;

	//if any of these conditions are met this effect will, as soon as possible, self terminate.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BASESETTINGS)
		TArray<TEnumAsByte<EndConditions>> EffectEndConditions;
#pragma endregion

#pragma region Health Settings
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Health)
		float HealthRegenPerSecond = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Health)
		float HealthDamagePerSecond = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Health)
		bool IsInvulnerable = false;

#pragma endregion

#pragma region Stamina Settings
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stamina)
		float StaminaRegenPerSecond = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stamina)
		float StaminaDamagePerSecond = 0.0f;

#pragma endregion

#pragma region Movement Settings
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		float MoveSpeedMod = 0.0f;

#pragma endregion

protected:
	FTimerHandle EffectTimerHandle;

	class UBuffDebuffSystem* m_OwningSystem;
};