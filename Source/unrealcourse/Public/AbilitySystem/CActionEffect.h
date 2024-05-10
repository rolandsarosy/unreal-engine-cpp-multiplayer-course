#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/CAction.h"
#include "CActionEffect.generated.h"

UCLASS(Blueprintable, Abstract)
class UNREALCOURSE_API UCActionEffect : public UCAction
{
	GENERATED_BODY()

public:
	virtual void StartAction_Implementation(AActor* Instigator) override;

	virtual void StopAction_Implementation(AActor* Instigator) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Effect")
	float Duration;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Effect")
	float Period;

	UFUNCTION(BlueprintNativeEvent, Category="Effect")
	void ExecutePeriodicEffect(AActor* Instigator);

private:
	FTimerHandle PeriodHandle;

	FTimerHandle DurationHandle;

	UCActionEffect();
};
