#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTask_HealSelfToPercentage.generated.h"

UCLASS()
class UNREALCOURSE_API UCBTTask_HealSelfToPercentage : public UBTTaskNode
{
	GENERATED_BODY()

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere, Category="AI")
	uint8 HealToPercentage;

private:
	static APawn* GetAIPawn(const UBehaviorTreeComponent& OwnerComp);
};
