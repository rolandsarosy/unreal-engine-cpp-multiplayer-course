#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CBTService_CheckAttackRange.generated.h"

UCLASS()
class UNREALCOURSE_API UCBTService_CheckAttackRange : public UBTService
{
	GENERATED_BODY()

public:
	UCBTService_CheckAttackRange();

protected:
	UPROPERTY(EditAnywhere, Category="Blackboard Keys", meta=(Tooltip = "The Blackboard key for the target actor to check the attack range and line of sight against."))
	FBlackboardKeySelector TargetActorKey;

	UPROPERTY(EditAnywhere, Category="Blackboard Keys", meta =(Tooltip = "The Blackboard key to put in the result boolean of this service."))
	FBlackboardKeySelector IsWithinAttackRangeKey;

	UPROPERTY(EditAnywhere, Category="AI", meta=(Tooltip = "The possible attack range of this entity to check the distance against the target with."))
	float AttackRange;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	static APawn* GetAIPawn(const UBehaviorTreeComponent& OwnerComp);

	AActor* GetTargetActor(const UBehaviorTreeComponent& OwnerComp) const;
};
