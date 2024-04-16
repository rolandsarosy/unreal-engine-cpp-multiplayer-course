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

	if (UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent(); ensure(BlackboardComponent))
	{
		APawn* AIPawn;
		const bool HasFoundPawn = GetAIPawn(OwnerComp, AIPawn);

		AActor* TargetActor;
		const bool HasFoundActor = GetTargetActor(BlackboardComponent, TargetActor);

		if (HasFoundPawn && HasFoundActor)
		{
			bool IsWithinAttackRange = FVector::Distance(TargetActor->GetActorLocation(), AIPawn->GetActorLocation()) <= AttackRange;
			bool HasLineOfSight = false;

			// This is not null-safe apparently, but I don't even care anymore. Do I really have to null-check every single call? Damn.
			if (IsWithinAttackRange) HasLineOfSight = Cast<AAIController>(OwnerComp.GetOwner())->LineOfSightTo(TargetActor); 
			
			BlackboardComponent->SetValueAsBool(IsWithinAttackRangeKey.SelectedKeyName, IsWithinAttackRange && HasLineOfSight);
		}
	}
}

// I'm really quite unsure about both of these methods. This is really weird that we need to check against nullability at
// each and every step of the flow. I'm not sure if these are all necessary. There isn't a definitive answer I found.
// If they are, then I really don't know how one can keep this up without writing unmaintainable code.
// Maybe guard clauses are the answer, I don't know, really. This is kinda disheartening.
//
// This would be one line if it wasn't for all this shit. 
bool UCBTService_CheckAttackRange::GetAIPawn(const UBehaviorTreeComponent& OwnerBehaviorTreeComponent, APawn*& OutPawn)
{
	OutPawn = nullptr;

	if (!OwnerBehaviorTreeComponent.GetAIOwner()) return false;

	OutPawn = OwnerBehaviorTreeComponent.GetAIOwner()->GetPawn();
	return OutPawn != nullptr;
}

bool UCBTService_CheckAttackRange::GetTargetActor(const UBlackboardComponent* BlackboardComponent, AActor*& OutActor) const
{
	OutActor = nullptr;

	UObject* BlackBoardValue = BlackboardComponent->GetValueAsObject(TargetActorKey.SelectedKeyName);
	if (!BlackBoardValue) return false;

	OutActor = Cast<AActor>(BlackBoardValue);
	return OutActor != nullptr;
}
