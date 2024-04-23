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

	const bool IsWithinAttackRange = FVector::Distance(TargetActor->GetActorLocation(), AIPawn->GetActorLocation()) <= AttackRange;
	bool HasLineOfSight = false;

	if (IsWithinAttackRange) HasLineOfSight = Cast<AAIController>(OwnerComp.GetOwner())->LineOfSightTo(TargetActor);

	OwnerComp.GetBlackboardComponent()->SetValueAsBool(IsWithinAttackRangeKey.SelectedKeyName, IsWithinAttackRange && HasLineOfSight);
}

APawn* UCBTService_CheckAttackRange::GetAIPawn(const UBehaviorTreeComponent& OwnerComp)
{
	const AAIController* AIController = OwnerComp.GetAIOwner();
	if (!ensure(AIController)) return nullptr;

	APawn* AIPawn = AIController->GetPawn();
	if (!ensure(AIPawn)) return nullptr;

	return AIPawn;
}

AActor* UCBTService_CheckAttackRange::GetTargetActor(const UBehaviorTreeComponent& OwnerComp) const
{
	const UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	if (!ensure(BlackboardComponent)) return nullptr;

	return Cast<AActor>(BlackboardComponent->GetValueAsObject(TargetActorKey.SelectedKeyName));
}
