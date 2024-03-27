// Fill out your copyright notice in the Description page of Project Settings.

#include "CCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"

ACCharacter::ACCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void ACCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ACCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	IsDebugEnabled ? VisualizeRotations() : void();
}

void ACCharacter::Move(const FInputActionInstance& InputActionInstance)
{
	FRotator ControlRotation = GetControlRotation();
	ControlRotation.Pitch = 0.0f;
	ControlRotation.Roll = 0.0f;

	const FVector2D InputAxisValue = InputActionInstance.GetValue().Get<FVector2D>();

	AddMovementInput(ControlRotation.Vector(), InputAxisValue.Y); // Forwards & Backwards
	AddMovementInput(FRotationMatrix(ControlRotation).GetScaledAxis(EAxis::Y), InputAxisValue.X); // Left & Right
}

void ACCharacter::Look(const FInputActionValue& InputActionValue)
{
	const FVector2D Value = InputActionValue.Get<FVector2D>();

	AddControllerYawInput(Value.X);
	AddControllerPitchInput(Value.Y);
}

void ACCharacter::ToggleIsDebugEnabled()
{
	IsDebugEnabled = !IsDebugEnabled;
}

void ACCharacter::VisualizeRotations() const
{
	constexpr float DrawScale = 50.0f;
	constexpr float Thickness = 2.0f;
	const FVector LineStart = GetActorLocation() += GetActorRightVector() * 100.0f; // Right-offset vector start from the actor

	const FVector ActorDirectionLineEnd = LineStart + (GetActorForwardVector() * 100.0f); // Vector's end from the actor
	DrawDebugDirectionalArrow(GetWorld(), LineStart, ActorDirectionLineEnd, DrawScale, FColor::Yellow, false, 0.0f, 0, Thickness);

	const FVector ControllerDirection_LineEnd = LineStart + (GetControlRotation().Vector() * 100.0f); // Vector's end from the controller
	DrawDebugDirectionalArrow(GetWorld(), LineStart, ControllerDirection_LineEnd, DrawScale, FColor::Green, false, 0.0f, 0, Thickness);
}

void ACCharacter::PrimaryAttack()
{
	// TODO: Make sure the socket name is not a burned-in value. We should probably expose this to the editor with either a socket or a string.
	const FTransform SpawnTransform = FTransform(GetControlRotation(), GetMesh()->GetSocketLocation("Muzzle_01"));

	FActorSpawnParameters SpawnParameters = FActorSpawnParameters();
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	GetWorld()->SpawnActor<AActor>(PrimaryProjectile, SpawnTransform, SpawnParameters);
}

// Called to bind functionality to input
void ACCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	const APlayerController* PlayerController = Cast<APlayerController>(GetController());
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	check(Subsystem);
	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(DefaultInputMapping, 0);

	EnhancedInputComponent->BindAction(Input_Move, ETriggerEvent::Triggered, this, &ACCharacter::Move);
	EnhancedInputComponent->BindAction(Input_Look, ETriggerEvent::Triggered, this, &ACCharacter::Look);
	EnhancedInputComponent->BindAction(Input_ToggleDebug, ETriggerEvent::Triggered, this, &ACCharacter::ToggleIsDebugEnabled);
	EnhancedInputComponent->BindAction(Input_PrimaryAttack, ETriggerEvent::Triggered, this, &ACCharacter::PrimaryAttack);
}
