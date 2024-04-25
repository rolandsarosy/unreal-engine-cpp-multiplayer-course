#include "AI/CAICharacter.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/CAttributeComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/PawnSensingComponent.h"

ACAICharacter::ACAICharacter()
{
	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComponent");
	AttributeComponent = CreateDefaultSubobject<UCAttributeComponent>("AttributeComponent");

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
}

void ACAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	PawnSensingComponent->OnSeePawn.AddDynamic(this, &ACAICharacter::OnPawnSeen);
	AttributeComponent->OnHealthChanged.AddDynamic(this, &ACAICharacter::OnHealthChanged);
}

// ReSharper disable once CppMemberFunctionMayBeConst ~ Incorrect suggestion
void ACAICharacter::OnPawnSeen(APawn* Pawn)
{
	// TODO: Consider not overriding the Target if it already has a target. Current behavior prevents the potentially fun behavior of them going to town on each other if they hit a friendly. 
	SetTargetActor(Pawn);
}

// ReSharper disable once CppMemberFunctionMayBeConst ~ Incorrect suggestion
// ReSharper disable once CppParameterMayBeConstPtrOrRef ~ Incorrect suggestion
void ACAICharacter::OnHealthChanged(AActor* InstigatorActor, UCAttributeComponent* UAttributeComponent, const float NewHealth, const float Delta)
{
	if (Delta < 0.0f)
	{
		if (InstigatorActor != this) SetTargetActor(InstigatorActor); // Set Instigator as target if entity was damaged by it.

		GetMesh()->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);
		
		if (NewHealth == 0) // Fucking DIES.
		{
			SetLifeSpan(5.0f);
			if (const AAIController* AIController = Cast<AAIController>(GetController())) AIController->BrainComponent->StopLogic("Got killed");
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			GetMesh()->SetCollisionProfileName("Ragdoll");
			GetMesh()->SetAllBodiesSimulatePhysics(true); // Enables ragdolling. 
		}
	}
}

void ACAICharacter::SetTargetActor(AActor* NewTarget) const
{
	if (AAIController* AIController = Cast<AAIController>(GetController()))
	{
		// I'm unsure how to get the Blackboard's values here in the Editor, since there is no clear BlackBoard in the context of the Character. TODO: Work this out properly in the future.
		AIController->GetBlackboardComponent()->SetValueAsObject("TargetActor", NewTarget);
	}
}
