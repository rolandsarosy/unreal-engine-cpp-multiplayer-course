#include "AI/CBTTask_HealSelfToPercentage.h"

#include "AIController.h"
#include "Components/CAttributeComponent.h"

EBTNodeResult::Type UCBTTask_HealSelfToPercentage::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* AIPawn = GetAIPawn(OwnerComp);
	if (!ensureMsgf(AIPawn, TEXT("BehaviorTreeTask was unable to find its AI Pawn."))) return EBTNodeResult::Failed;

	UCAttributeComponent* AttributeComponent = UCAttributeComponent::GetComponentFrom(AIPawn);
	if (!ensureMsgf(AttributeComponent, TEXT("BehaviorTreeTask for healing tried to run on a AI Pawn with no relevant component."))) return EBTNodeResult::Failed;

	const uint8 CurrentHealthPercentage = (AttributeComponent->GetHealthCurrent() / AttributeComponent->GetHealthMax()) * 100;

	if (CurrentHealthPercentage >= HealToPercentage) return EBTNodeResult::Failed; // According to the documentation, this could very well be ABORTED as well. I'm not sure which is the better yet.	

	AttributeComponent->ApplyHealthChange(AIPawn, AttributeComponent->GetHealthMax() * (static_cast<float>(HealToPercentage - CurrentHealthPercentage) / 100));
	return EBTNodeResult::Succeeded;
}

APawn* UCBTTask_HealSelfToPercentage::GetAIPawn(const UBehaviorTreeComponent& OwnerComp)
{
	const AAIController* AIController = OwnerComp.GetAIOwner();
	if (!ensure(AIController)) return nullptr;

	APawn* AIPawn = AIController->GetPawn();
	if (!ensure(AIPawn)) return nullptr;

	return AIPawn;
}
