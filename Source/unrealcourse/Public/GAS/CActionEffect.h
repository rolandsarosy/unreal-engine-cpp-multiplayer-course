#pragma once

#include "CoreMinimal.h"
#include "GAS/CBaseAction.h"
#include "CActionEffect.generated.h"

/**
 * @class UCActionEffect
 *
 * @brief Represents an action effect that can be applied to an actor.
 *
 * The UCActionEffect class is a subclass of UCBaseAction. It provides functionality to start and stop an action effect, as well as execute a periodic effect at regular intervals.
 * The duration and period of the effect can be configured, and the effect will automatically stop once the duration elapses.
 */
UCLASS(Blueprintable, Abstract)
class UNREALCOURSE_API UCActionEffect : public UCBaseAction
{
	GENERATED_BODY()

public:
	UCActionEffect();

	virtual void StartAction_Implementation(AActor* Instigator) override;

	virtual void StopAction_Implementation(AActor* Instigator) override;

protected:
	/**
	 * @brief The duration of the action effect.
	 *
	 * The duration specifies the length of time, in seconds, that the action effect will last. Once the duration elapses, the action effect will automatically stop.
	 *
	 * @see UCActionEffect::StartAction_Implementation()
	 * @see UCActionEffect::StopAction_Implementation()
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Effect")
	float Duration;

	/**
	 * @brief The period of the action effect.
	 *
	 * The period specifies the interval, in seconds, at which the periodic effect will be executed. The effect will be executed repeatedly with this interval until the action effect is stopped.
	 *
	 * @see UCActionEffect::StartAction_Implementation()
	 * @see UCActionEffect::ExecutePeriodicEffect()
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Effect")
	float Period;

	/**
	 * @brief Executes a periodic effect.
	 *
	 * BlueprintImplementable event that executes a periodic effect. It is called at regular intervals defined by the period, until the action effect is stopped.
	 *
	 * @param Instigator The actor that triggered the periodic effect.
	 *
	 * @see UCActionEffect::StartAction_Implementation()
	 * @see UCActionEffect::StopAction_Implementation()
	 */
	UFUNCTION(BlueprintImplementableEvent, Category="Effect")
	void ExecutePeriodicEffect(AActor* Instigator);

	UFUNCTION(BlueprintCallable, Category="Effect")
	float GetTimeRemaining() const;

private:
	FTimerHandle PeriodHandle;

	FTimerHandle DurationHandle;
};
