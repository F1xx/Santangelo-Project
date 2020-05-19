// Fill out your copyright notice in the Description page of Project Settings.


#include "SAPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

// Sets default values
ASAPlayer::ASAPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

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
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

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
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Val);
	}
}

void ASAPlayer::MoveRight(float Val)
{
	if (Val != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Val);
	}
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