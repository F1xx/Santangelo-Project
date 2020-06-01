// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDamageDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDeathDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SANTANGELO_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void OnDeath();
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
		void TakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION(BlueprintCallable)
		FORCEINLINE float GetHealth() { return CurrentHealth; }

	UFUNCTION(BlueprintCallable)
		FORCEINLINE float GetHealthPercent() { return CurrentHealth / MaxHealth; }

	UFUNCTION(BlueprintCallable)
		FORCEINLINE bool IsAlive() { return CurrentHealth > 0.0f; }

	UPROPERTY(BlueprintAssignable, Category = "Events")
		FDamageDelegate OnDamageDelegate;
	UPROPERTY(BlueprintAssignable, Category = "Events")
		FDeathDelegate OnDeathDelegate;

protected:
	UPROPERTY(BlueprintReadOnly, Category = Health)
		float CurrentHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Health)
		float MaxHealth = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Health)
		float RegenDelay = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Health)
		float RegenPerSecond = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Health)
		bool CanRegen = true;

	FTimerHandle RegenDelayHandle;
};