#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CBTService_CheckSelfForLowHealth.generated.h"

UCLASS()
class UNREALCOURSE_API UCBTService_CheckSelfForLowHealth : public UBTService
{
	GENERATED_BODY()

public:
	UCBTService_CheckSelfForLowHealth();

protected:
	UPROPERTY(EditAnywhere, Category="Blackboard Keys", meta=(Tooltip = "This Blackboard key will be set every time the service runs, to check whether the AI entity is on critical health or not."))
	FBlackboardKeySelector IsOnLowHealthKey;

	UPROPERTY(EditAnywhere, Category="AI",meta=(ClampMin = "1", ClampMax = "99", Tooltip = "The health percentage (inclusive, between 0-100) breakpoint that'll determine where the AI entity is considered to be critically low on health."))
	uint8 LowHealthPercentageBreakPoint;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	static APawn* GetAIPawn(const UBehaviorTreeComponent& OwnerComp);
};
