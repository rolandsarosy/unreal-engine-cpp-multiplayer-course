#include "AI/CBTService_CheckAttackRange.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UCBTService_CheckAttackRange::UCBTService_CheckAttackRange()
{
	AttackRange = 1000.0f;
}

void UCBTService_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	const APawn* AIPawn = GetAIPawn(OwnerComp);
	const AActor* TargetActor = GetTargetActor(OwnerComp);
	if (!AIPawn || !TargetActor) return;

	const bool bIsWithinAttackRange = FVector::Distance(TargetActor->GetActorLocation(), AIPawn->GetActorLocation()) <= AttackRange;
	bool bHasLineOfSight = false;

	if (bIsWithinAttackRange) bHasLineOfSight = Cast<AAIController>(OwnerComp.GetOwner())->LineOfSightTo(TargetActor);

	OwnerComp.GetBlackboardComponent()->SetValueAsBool(IsWithinAttackRangeKey.SelectedKeyName, bIsWithinAttackRange && bHasLineOfSight);
}

APawn* UCBTService_CheckAttackRange::GetAIPawn(const UBehaviorTreeComponent& OwnerComp)
{
	const AAIController* AIController = OwnerComp.GetAIOwner();
	if (!ensure(AIController)) return nullptr;

	// Ensuring here is counter-productive as the AIController isn't guaranteed to have a Pawn because the BehaviorTree gets to work sooner than the controller possesses the pawn.
	return AIController->GetPawn();
}

AActor* UCBTService_CheckAttackRange::GetTargetActor(const UBehaviorTreeComponent& OwnerComp) const
{
	const UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	if (!ensure(BlackboardComponent)) return nullptr;

	return Cast<AActor>(BlackboardComponent->GetValueAsObject(TargetActorKey.SelectedKeyName));
}
