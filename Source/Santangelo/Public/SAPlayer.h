// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SAPlayer.generated.h"

UCLASS(config = Game)
class SANTANGELO_API ASAPlayer : public ACharacter
{
	GENERATED_BODY()

		/** First person camera */
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FPCameraComponent;

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USkeletalMeshComponent* ArmsMeshFP;

	FTimerHandle DodgeCooldownHandle;
public:
	// Sets default values for this character's properties
	ASAPlayer();
	~ASAPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stamina)
		class UStaminaComponent* StaminaComp;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Health)
		class UHealthComponent* HealthComp;

public:
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
		float BaseLookUpRate;

protected:
	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	void MoveRight(float Val);

	void Jump() override;

	void StopJumping() override;

	void Dodge();

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Returns ArmsMeshFP subobject **/
	FORCEINLINE class USkeletalMeshComponent* GetArmsMeshFP() const { return ArmsMeshFP; }
	/** Returns FPCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetFPCameraComponent() const { return FPCameraComponent; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stamina)
		float DodgeCooldown = 1.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (ClampMin = "1.0", ClampMax = "10.0", UIMin = "1.0", UIMax = "10.0"))
		float SprintMult = 1.7f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		float DodgeStrength;

	float BaseWalkSpeed = 1000.0f;
};
