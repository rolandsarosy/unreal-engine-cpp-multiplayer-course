#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CAnimInstance.generated.h"

class UCAttributeComponent;
class UCActionComponent;

UCLASS()
class UNREALCOURSE_API UCAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UCActionComponent> ActionComponent;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UCAttributeComponent> AttributeComponent;

private:
	virtual void NativeInitializeAnimation() override;
};
