#include "CCharacter.h"

#include "CGameplayFunctionLibrary.h"
#include "Components/CAttributeComponent.h"
#include "Components/CInteractionComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

ACCharacter::ACCharacter()
{
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);

	InteractionComponent = CreateDefaultSubobject<UCInteractionComponent>("InteractionComponent");
	AttributeComponent = CreateDefaultSubobject<UCAttributeComponent>("AttributeComponent");

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
	DisableInput(Cast<APlayerController>(GetController()));
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

void ACCharacter::PrimaryAttack_Start()
{
	PlayAnimMontage(AttackAnimation);

	// TODO: This is merely temporary, and animation notifies will be used here in the future.
	GetWorldTimerManager().SetTimer(TimerHandle_Attack, this, &ACCharacter::PrimaryAttack_TimeElapsed, 0.2F);
}

void ACCharacter::PrimaryAttack_TimeElapsed()
{
	const FTransform SpawnTransform = FTransform(TraceForProjectileSpawnRotator(), GetMesh()->GetSocketLocation(AttackSocketName));
	FActorSpawnParameters SpawnParameters = FActorSpawnParameters();
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParameters.Instigator = this;

	UGameplayStatics::SpawnEmitterAttached(MuzzleFlashParticleSystem, GetMesh(), GetMesh()->GetSocketBoneName(AttackSocketName));
	GetWorld()->SpawnActor<AActor>(PrimaryAttackProjectile, SpawnTransform, SpawnParameters);
}

void ACCharacter::SpecialAttack_Start()
{
	PlayAnimMontage(AttackAnimation);

	// TODO: This is merely temporary, and animation notifies will be used here in the future.
	GetWorldTimerManager().SetTimer(TimerHandle_Attack, this, &ACCharacter::SpecialAttack_TimeElapsed, 0.2F);
}

void ACCharacter::SpecialAttack_TimeElapsed()
{
	const FTransform SpawnTransform = FTransform(TraceForProjectileSpawnRotator(), GetMesh()->GetSocketLocation(AttackSocketName));
	FActorSpawnParameters SpawnParameters = FActorSpawnParameters();
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParameters.Instigator = this;

	UGameplayStatics::SpawnEmitterAttached(MuzzleFlashParticleSystem, GetMesh(), GetMesh()->GetSocketBoneName(AttackSocketName));
	GetWorld()->SpawnActor<AActor>(SpecialAttackProjectile, SpawnTransform, SpawnParameters);
}

void ACCharacter::TeleportAttack_Start()
{
	PlayAnimMontage(AttackAnimation);

	// TODO: This is merely temporary, and animation notifies will be used here in the future.
	GetWorldTimerManager().SetTimer(TimerHandle_Attack, this, &ACCharacter::TeleportAttack_TimeElapsed, 0.2F);
}

void ACCharacter::TeleportAttack_TimeElapsed()
{
	const FTransform SpawnTransform = FTransform(TraceForProjectileSpawnRotator(), GetMesh()->GetSocketLocation(AttackSocketName));
	FActorSpawnParameters SpawnParameters = FActorSpawnParameters();
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParameters.Instigator = this;

	UGameplayStatics::SpawnEmitterAttached(MuzzleFlashParticleSystem, GetMesh(), GetMesh()->GetSocketBoneName(AttackSocketName));
	GetWorld()->SpawnActor<AActor>(TeleportAttackProjectile, SpawnTransform, SpawnParameters);
}

/**
 * @brief Traces for the rotation to be used when spawning a projectile.
 *
 * @details This method performs a line trace from the camera component to find the target location for spawning a projectile.
 * It checks for collisions with specified object types and returns the rotation required to face the target location.
 * If the line trace does not result in a blocking hit, the rotation will be calculated based on the end location of the trace.
 *
 * @return The rotation to be used when spawning a projectile.
 */
FRotator ACCharacter::TraceForProjectileSpawnRotator() const
{
	FHitResult TraceHitResult;
	FVector TraceStart = CameraComponent->GetComponentLocation();
	FVector TraceEnd = CameraComponent->GetComponentLocation() + CameraComponent->GetForwardVector() * 5000;
	FCollisionObjectQueryParams QueryParams = FCollisionObjectQueryParams();
	FCollisionQueryParams TraceParams(FName("Actor Self-Ignore trace parameter"), true, this);

	QueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	QueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
	QueryParams.AddObjectTypesToQuery(ECC_Pawn);
	QueryParams.AddObjectTypesToQuery(ECC_PhysicsBody);

	bool bIsTraceBlockingHit = GetWorld()->LineTraceSingleByObjectType(TraceHitResult, TraceStart, TraceEnd, QueryParams, TraceParams);
	FVector SpawnRotatorTarget = bIsTraceBlockingHit ? TraceHitResult.ImpactPoint : TraceEnd; // Handle cases where the tracing did not result in a blocking hit.
	return UKismetMathLibrary::FindLookAtRotation(GetMesh()->GetSocketLocation(AttackSocketName), SpawnRotatorTarget);
}

FVector ACCharacter::GetPawnViewLocation() const
{
	return CameraComponent->GetComponentLocation();
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
	EnhancedInputComponent->BindAction(Input_PrimaryInteract, ETriggerEvent::Triggered, InteractionComponent.Get(), &UCInteractionComponent::PrimaryInteract);
	EnhancedInputComponent->BindAction(Input_PrimaryAttack, ETriggerEvent::Triggered, this, &ACCharacter::PrimaryAttack_Start);
	EnhancedInputComponent->BindAction(Input_SpecialAttack, ETriggerEvent::Triggered, this, &ACCharacter::SpecialAttack_Start);
	EnhancedInputComponent->BindAction(Input_TeleportAttack, ETriggerEvent::Triggered, this, &ACCharacter::TeleportAttack_Start);
	EnhancedInputComponent->BindAction(Input_Jump, ETriggerEvent::Triggered, this, &ACCharacter::Jump);
}

// This is a cheat that'll work only in non-shipped builds. Conditional compiling is unnecessary as the console is disabled in shipped builds.
void ACCharacter::HealSelf(const float Amount)
{
	UCGameplayFunctionLibrary::ApplyHealing(this, this, Amount);
}
