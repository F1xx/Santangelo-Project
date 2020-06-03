// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "BuffDebuffSystem.generated.h"

class UEffect;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SANTANGELO_API UBuffDebuffSystem : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBuffDebuffSystem();
	~UBuffDebuffSystem();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void HandleMemoryManagement();

	void RemoveEffect(UEffect* effect);

public:	
	UFUNCTION()
		void QueueForRemoval(UEffect* effect);
	UFUNCTION()
		void QueueForAddition(UEffect* effect);
	UFUNCTION()
		void ClearEffects();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FORCEINLINE const TArray<class UEffect*>& GetAllEffects() { return m_Effects; }

protected:
	TArray<class UEffect*> m_Effects;

	TArray<UEffect*> m_EffectsToRemove;
	TArray<UEffect*> m_EffectsToAdd;
};