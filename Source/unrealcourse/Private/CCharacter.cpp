#include "CCharacter.h"

#include "AbilitySystem/CActionComponent.h"
#include "Components/CAttributeComponent.h"
#include "Components/CInteractionComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ACCharacter::ACCharacter()
{
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);

	InteractionComponent = CreateDefaultSubobject<UCInteractionComponent>("InteractionComponent");
	AttributeComponent = CreateDefaultSubobject<UCAttributeComponent>("AttributeComponent");
	ActionComponent = CreateDefaultSubobject<UCActionComponent>("ActionComponent");

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void ACCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	AttributeComponent->OnHealthChanged.AddDynamic(this, &ACCharacter::OnHealthChanged);
	AttributeComponent->OnDeath.AddDynamic(this, &ACCharacter::OnDeath);
}

// ReSharper disable once CppMemberFunctionMayBeConst - Incorrect suggestion
void ACCharacter::OnHealthChanged(AActor* Actor, UCAttributeComponent* UAttributeComponent, const float NewHealth, const float Delta)
{
	if (Delta < 0.0f) GetMesh()->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);
}

void ACCharacter::OnDeath(AActor* KillerActor, UCAttributeComponent* OwnerComponent)
{
	SetLifeSpan(25.0f);
	DisableInput(Cast<APlayerController>(GetController()));
	GetCharacterMovement()->DisableMovement();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
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

void ACCharacter::SprintStart() { ActionComponent->StartActionByTag(this, ActionTag_Sprint); }

void ACCharacter::SprintStop() { ActionComponent->StopActionByTag(this, ActionTag_Sprint); }

void ACCharacter::PrimaryAttack() { ActionComponent->StartActionByTag(this, ActionTag_PrimaryAttack); }

void ACCharacter::SpecialAttack() { ActionComponent->StartActionByTag(this, ActionTag_SpecialAttack); }

void ACCharacter::TeleportAttack() { ActionComponent->StartActionByTag(this, ActionTag_TeleportAttack); }

void ACCharacter::Parry() { ActionComponent->StartActionByTag(this, ActionTag_Parry); }

FVector ACCharacter::GetPawnViewLocation() const { return CameraComponent->GetComponentLocation(); }

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
	EnhancedInputComponent->BindAction(Input_PrimaryInteract, ETriggerEvent::Triggered, InteractionComponent.Get(), &UCInteractionComponent::PrimaryInteract);
	EnhancedInputComponent->BindAction(Input_PrimaryAttack, ETriggerEvent::Triggered, this, &ACCharacter::PrimaryAttack);
	EnhancedInputComponent->BindAction(Input_SpecialAttack, ETriggerEvent::Triggered, this, &ACCharacter::SpecialAttack);
	EnhancedInputComponent->BindAction(Input_TeleportAttack, ETriggerEvent::Triggered, this, &ACCharacter::TeleportAttack);
	EnhancedInputComponent->BindAction(Input_Jump, ETriggerEvent::Triggered, this, &ACCharacter::Jump);
	EnhancedInputComponent->BindAction(Input_Sprint, ETriggerEvent::Started, this, &ACCharacter::SprintStart);
	EnhancedInputComponent->BindAction(Input_Sprint, ETriggerEvent::Completed, this, &ACCharacter::SprintStop);
	EnhancedInputComponent->BindAction(Input_Parry, ETriggerEvent::Triggered, this, &ACCharacter::Parry);
}
