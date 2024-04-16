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
	UPROPERTY(EditAnywhere, Category="Blackboard Keys")
	FBlackboardKeySelector TargetActorKey;

	UPROPERTY(EditAnywhere, Category="Blackboard Keys")
	FBlackboardKeySelector IsWithinAttackRangeKey;

	UPROPERTY(EditAnywhere, Category="AI")
	float AttackRange;
	
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	static bool GetAIPawn(const UBehaviorTreeComponent& OwnerBehaviorTreeComponent, APawn*& OutPawn);

	bool GetTargetActor(const UBlackboardComponent* BlackboardComponent, AActor*& OutActor) const;
};
