#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CAIController.generated.h"

class UBehaviorTree;

UCLASS(Abstract)
class UNREALCOURSE_API ACAIController : public AAIController
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category="AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	virtual void BeginPlay() override;
};
