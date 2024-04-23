/*
* This file, originally forked from the BTDecoratorCooldown files of the Unreal Engine source code,
 * contains modifications made to rectify an issue where cooldown would prematurely commence even without invocation.
 * 
 * The applied solution closely mirrors the implementation of BTDecoratorCooldown in Unreal Engine version 5.4.
 * All digital rights of this file are hereby attributed to the original sources.
 *
 * PLEASE NOTE: This file is provided "as is", without warranty of any kind, express or implied. 
 * The author of this file does not hold any digital rights over it. 
 */

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "CBTDecorator_Cooldown.generated.h"

struct FCBTCooldownDecoratorMemory
{
	double LastUseTimestamp;
	uint8 bRequestedRestart : 1;
};

/**
 * A custom cooldown decorator node.
 * A decorator node that bases its condition on whether a cooldown timer has expired. Compared to the original version in UE5.3, it resolved the issue where the cooldown would
 * prematurely commence without invocation.
 */
UCLASS(HideCategories=(Condition))
class UNREALCOURSE_API UCBTDecorator_Cooldown : public UBTDecorator
{
	GENERATED_BODY()

	UCBTDecorator_Cooldown();

	/** Specifies the cooldown time of this decorator. */
	UPROPERTY(EditAnywhere, Category=Decorator)
	float CooldownTime;

	virtual void PostLoad() override;
	virtual void InitializeMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryInit::Type InitType) const override;
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	virtual void DescribeRuntimeValues(const UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTDescriptionVerbosity::Type Verbosity, TArray<FString>& Values) const override;
	virtual uint16 GetInstanceMemorySize() const override;
	virtual FString GetStaticDescription() const override;

protected:
	virtual void OnNodeDeactivation(FBehaviorTreeSearchData& SearchData, EBTNodeResult::Type NodeResult) override;
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

public:
#if WITH_EDITOR
	virtual FName GetNodeIconName() const override;
#endif
};
