#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTask_RangedAttack.generated.h"

UCLASS()
class UNREALCOURSE_API UCBTTask_RangedAttack : public UBTTaskNode
{
	GENERATED_BODY()

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere, Category="Blackboard Keys", meta=(Tooltip = "This Blackboard key represents the target to make the ranged attack towards."))
	FBlackboardKeySelector TargetActorKey;

	UPROPERTY(EditAnywhere, Category="Projectile", meta=(Tooltip = "This is the Projectile that'll be spawned when making the ranged attack towards the target."))
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditAnywhere, Category="Projectile", meta=(Tooltip = "This is the Skeletal Mesh Socket's name where the projectile will spawn from, towards the target."))
	FName AttackSocketName;

private:
	AActor* GetTargetActor(const UBehaviorTreeComponent& OwnerComp) const;

	static ACharacter* GetAICharacter(const UBehaviorTreeComponent& OwnerComp);
};
