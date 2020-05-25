// Fill out your copyright notice in the Description page of Project Settings.


#include "SAPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "StaminaComponent.h"
#include "HealthComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

// Sets default values
ASAPlayer::ASAPlayer() :
	BaseTurnRate(45.0f)
	, BaseLookUpRate(45.0f)
	, DodgeStrength(1000.0f)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// Create a CameraComponent	
	FPCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FPCameraComponent->SetupAttachment(GetCapsuleComponent());
	FPCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FPCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	ArmsMeshFP = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ArmsMeshFP"));
	ArmsMeshFP->SetOnlyOwnerSee(true);
	ArmsMeshFP->SetupAttachment(FPCameraComponent);
	ArmsMeshFP->bCastDynamicShadow = false;
	ArmsMeshFP->CastShadow = false;
	ArmsMeshFP->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	ArmsMeshFP->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));


	UCharacterMovementComponent* moveComp = GetCharacterMovement();
	BaseWalkSpeed = moveComp->MaxWalkSpeed;
	//moveComp->bOrientRotationToMovement = true;
	//moveComp->AirControl = 0.1f;
	//moveComp->RotationRate = FRotator(0.0f, 540.0f, 0.0f);


	StaminaComp = CreateDefaultSubobject<UStaminaComponent>(TEXT("StaminaComp"));

	HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
}

ASAPlayer::~ASAPlayer()
{

}

// Called when the game starts or when spawned
void ASAPlayer::BeginPlay()
{
	Super::BeginPlay();

	//ArmsMeshFP->SetHiddenInGame(false, true);
}

// Called every frame
void ASAPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ASAPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASAPlayer::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ASAPlayer::StopJumping);

	//Sprint
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, StaminaComp, &UStaminaComponent::Sprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, StaminaComp, &UStaminaComponent::StopSprinting);

	//dodging
	PlayerInputComponent->BindAction("Dodge", IE_Pressed, this, &ASAPlayer::Dodge);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &ASAPlayer::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASAPlayer::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ASAPlayer::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ASAPlayer::LookUpAtRate);
}

void ASAPlayer::MoveForward(float Val)
{
	if (Val != 0.0f)
	{
		UCharacterMovementComponent* moveComp = GetCharacterMovement();
		if (StaminaComp->IsSprinting())
		{
			moveComp->MaxWalkSpeed = BaseWalkSpeed * SprintMult;
		}
		else if(moveComp->MaxWalkSpeed != BaseWalkSpeed)
		{
			moveComp->MaxWalkSpeed = BaseWalkSpeed;
		}

		// add movement in that direction
		//scale the Val for sprinting
		AddMovementInput(GetActorForwardVector(), Val);
	}
}

void ASAPlayer::MoveRight(float Val)
{
	if (Val != 0.0f)
	{
		UCharacterMovementComponent* moveComp = GetCharacterMovement();
		if (StaminaComp->IsSprinting() && StaminaComp->IsStaminaDepleted() == false)
		{
			moveComp->MaxWalkSpeed = BaseWalkSpeed * SprintMult;
		}
		else if (StaminaComp->IsSprinting() && StaminaComp->IsStaminaDepleted() == true)
		{
			moveComp->MaxWalkSpeed = BaseWalkSpeed * (SprintMult * 0.75);
		}
		else if (moveComp->MaxWalkSpeed != BaseWalkSpeed)
		{
			moveComp->MaxWalkSpeed = BaseWalkSpeed;
		}

		// add movement in that direction
		//scale the Val for sprinting
		AddMovementInput(GetActorRightVector(), Val);
	}
}

void ASAPlayer::Jump()
{
	//only jump if stamina says we can
	if (StaminaComp->Jump())
	{
		Super::Jump();
	}
}

void ASAPlayer::StopJumping()
{
	Super::StopJumping();
}

void ASAPlayer::Dodge()
{
	//Check if we're on the ground or on cooldown
	if (GetCharacterMovement()->IsFalling() || GetWorld()->GetTimerManager().IsTimerActive(DodgeCooldownHandle))
	{
		return;
	}

	//check if we have stamina, if not make it a weaker dodge
	bool hasEnoughStamina = StaminaComp->Dodge();

	//consume stamina (stamina can go negative)

	FVector V = GetVelocity();

	float currentDodgeStrength = DodgeStrength;

	if (hasEnoughStamina == false)
	{
		currentDodgeStrength *= 0.75f;
	}

	if (V.IsZero())
	{
		//this will be used to invert the forward vector if we are standing still
		//needs -dodgestrength instead of just like -1 otherwise its too weak
		V = GetActorForwardVector() * -currentDodgeStrength;

		FVector forward = GetActorForwardVector();
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, FString::Printf(TEXT("forward: %f, %f, %f"), forward.X, forward.Y, forward.Z));

	}

	V *= currentDodgeStrength;

	//add impulse
	GetCharacterMovement()->AddImpulse(V);

	//start cooldown (should cooldown be linked to animation? Should cooldown be related to weight?)
	GetWorld()->GetTimerManager().SetTimer(DodgeCooldownHandle, nullptr, DodgeCooldown, false);
}

void ASAPlayer::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ASAPlayer::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}