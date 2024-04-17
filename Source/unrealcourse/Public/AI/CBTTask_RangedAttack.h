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
	UPROPERTY(EditAnywhere, Category="Blackboard Keys")
	FBlackboardKeySelector TargetActorKey;

	UPROPERTY(EditAnywhere, Category="Projectile")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditAnywhere, Category="Projectile")
	FName AttackSocketName;
	
	AActor* GetTargetActor(const UBehaviorTreeComponent& OwnerComp) const;

	static ACharacter* GetAICharacter(const UBehaviorTreeComponent& OwnerComp);
};
