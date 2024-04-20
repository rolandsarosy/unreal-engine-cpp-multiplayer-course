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
	UPROPERTY(EditAnywhere, Category="AI", meta=(ClampMin="1", ClampMax="100", Tooltip = "The AI entity will be healed to this percentage of its total health upon calling this class."))
	uint8 HealToPercentage;

private:
	static APawn* GetAIPawn(const UBehaviorTreeComponent& OwnerComp);
};
