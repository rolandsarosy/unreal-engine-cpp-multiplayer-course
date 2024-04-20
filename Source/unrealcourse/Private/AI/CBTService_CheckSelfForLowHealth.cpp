#include "AI/CBTService_CheckSelfForLowHealth.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CAttributeComponent.h"

// The better way to check for low health would be to check every time when the AI entity's health changes. This only happens periodically, which is unnecessary and does not cover all cases.
// This however, was what Assignment 4 specified. Maybe I'll fix it in the future if it doesn't happen during class.
UCBTService_CheckSelfForLowHealth::UCBTService_CheckSelfForLowHealth()
{
	LowHealthPercentageBreakPoint = 30;
}

void UCBTService_CheckSelfForLowHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	const APawn* AIPawn = GetAIPawn(OwnerComp);
	if (!ensureMsgf(AIPawn, TEXT("BehaviorTreeService was unable to find its AI Pawn."))) return;

	const UCAttributeComponent* AttributeComponent = Cast<UCAttributeComponent>(AIPawn->GetComponentByClass(UCAttributeComponent::StaticClass()));
	if (!ensureMsgf(AttributeComponent, TEXT("BehaviorTreeService for low health checking tried to run on a AI Pawn with no relevant component."))) return;

	const uint8 CurrentHealthPercentage = (AttributeComponent->GetHealthCurrent() / AttributeComponent->GetHealthMax()) * 100;

	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	if (!ensureMsgf(BlackboardComponent, TEXT("BehaviorTreeService could not find relevant BlackBoardComponent to set values in."))) return;

	const bool IsOnLowHealth = CurrentHealthPercentage <= LowHealthPercentageBreakPoint;
	BlackboardComponent->SetValueAsBool(IsOnLowHealthKey.SelectedKeyName, IsOnLowHealth);
}

APawn* UCBTService_CheckSelfForLowHealth::GetAIPawn(const UBehaviorTreeComponent& OwnerComp)
{
	const AAIController* AIController = OwnerComp.GetAIOwner();
	if (!ensure(AIController)) return nullptr;

	APawn* AIPawn = AIController->GetPawn();
	if (!ensure(AIPawn)) return nullptr;

	return AIPawn;
}
