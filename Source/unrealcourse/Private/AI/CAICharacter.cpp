#include "AI/CAICharacter.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "Framework/CCharacter.h"
#include "CWorldUserWidget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/CActionComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/CAttributeComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "CWorldHealthBar.h"

ACAICharacter::ACAICharacter()
{
	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComponent");
	AttributeComponent = CreateDefaultSubobject<UCAttributeComponent>("AttributeComponent");
	ActionComponent = CreateDefaultSubobject<UCActionComponent>("ActionComponent");

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;

	CoinRewardUponDeath = 0;

	bHasSeenPlayers = false;
}

void ACAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	PawnSensingComponent->OnSeePawn.AddDynamic(this, &ACAICharacter::OnSeePawn);
	AttributeComponent->OnHealthChanged.AddDynamic(this, &ACAICharacter::OnHealthChanged);
	AttributeComponent->OnDeath.AddDynamic(this, &ACAICharacter::OnDeath);
}

// ReSharper disable once CppMemberFunctionMayBeConst ~ Incorrect suggestion
void ACAICharacter::OnHealthChanged(AActor* InstigatorActor, UCAttributeComponent* UAttributeComponent, const float NewHealth, const float Delta)
{
	if (Delta < 0.0f)
	{
		GetMesh()->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);

		if (HasAuthority() && InstigatorActor != this)
		{
			SetTargetActor(Cast<APawn>(InstigatorActor), true);
			MutlicastAddSpottedWidgetConditionally(InstigatorActor);
			MulticastAddHealthBar(NewHealth, AttributeComponent);
		}
	}
}

void ACAICharacter::OnDeath(AActor* KillerActor, UCAttributeComponent* OwnerComponent)
{
	SetLifeSpan(3.5f);
	if (const AAIController* AIController = Cast<AAIController>(GetController())) AIController->BrainComponent->StopLogic("Got killed");
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->DisableMovement();
	GetMesh()->SetCollisionProfileName("Ragdoll");
	GetMesh()->SetAllBodiesSimulatePhysics(true);
}

/**
 * @brief Sets the target actor for the AI character.
 *
 * This method is used to set the target actor for the AI character, which is responsible for tracking and engaging the target.
 *
 * @param NewTarget     The new target actor to set.
 * @param bShouldOverrideCurrentTarget    Flag indicating whether the current target should be overridden even if it is alive.
 *
 */
void ACAICharacter::SetTargetActor(AActor* NewTarget, const bool bShouldOverrideCurrentTarget) const
{
	// I'm unsure how to get the Blackboard's values here in the Editor, since there is no clear BlackBoard in the context of the Character. TODO: Work this out properly in the future.
	const FName BlackboardKeyName = TEXT("TargetActor");

	AAIController* AIController = Cast<AAIController>(GetController());
	if (!ensure(AIController)) return;

	UBlackboardComponent* BlackboardComponent = AIController->GetBlackboardComponent();
	if (!ensure(BlackboardComponent)) return;

	AActor* CurrentTarget = Cast<AActor>(BlackboardComponent->GetValueAsObject(BlackboardKeyName));

	bool bIsCurrentTargetAlive = false;
	if (CurrentTarget)
	{
		if (const UCAttributeComponent* CurrentTargetAttributeComponent = UCAttributeComponent::GetComponentFrom(CurrentTarget)) bIsCurrentTargetAlive = CurrentTargetAttributeComponent->IsAlive();
	}

	if (bShouldOverrideCurrentTarget || !CurrentTarget || !bIsCurrentTargetAlive)
	{
		BlackboardComponent->SetValueAsObject(BlackboardKeyName, NewTarget);
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst - Incorrect suggestion 
void ACAICharacter::OnSeePawn(APawn* Pawn)
{
	SetTargetActor(Pawn, false);
	MutlicastAddSpottedWidgetConditionally(Pawn);
}

void ACAICharacter::MulticastAddHealthBar_Implementation(const float InitialHealth, UCAttributeComponent* OwnerAttributeComponent)
{
	if (ActiveHealthBar == nullptr)
	{
		ActiveHealthBar = CreateWidget<UCWorldHealthBar>(GetWorld(), HealthBarWidgetClass);
		if (ActiveHealthBar)
		{
			ActiveHealthBar->AttachedActor = this;
			ActiveHealthBar->InitialHealth = InitialHealth;
			ActiveHealthBar->AttributeComponent = OwnerAttributeComponent;
			ActiveHealthBar->AddToViewport();
		}
	}
}

void ACAICharacter::MutlicastAddSpottedWidgetConditionally_Implementation(const AActor* InstigatorActor)
{
	if (bHasSeenPlayers || !InstigatorActor->IsA(ACCharacter::StaticClass())) return;

	UCWorldUserWidget* Widget = CreateWidget<UCWorldUserWidget>(GetWorld(), SpottedPopupWidgetClass);
	Widget->AttachedActor = this;
	Widget->AddToViewport(10);

	bHasSeenPlayers = true;
}

void ACAICharacter::SetCoinRewardUponDeath(const int NewCoinRewardUponDeath) { CoinRewardUponDeath = NewCoinRewardUponDeath; }

int ACAICharacter::GetCoinRewardUponDeath() const { return CoinRewardUponDeath; }
