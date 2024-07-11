#include "GAS/CActionEffect.h"
#include "Components/CActionComponent.h"

UCActionEffect::UCActionEffect()
{
	bAutoStart = true;
	Duration = 3.0f;
	Period = 0.25f;
}

void UCActionEffect::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	if (Duration > 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(DurationHandle, [this, Instigator] { StopAction(Instigator); }, Duration, false);
	}

	if (Period > 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(PeriodHandle, [this, Instigator] { ExecutePeriodicEffect(Instigator); }, Period, true);
	}
}

void UCActionEffect::StopAction_Implementation(AActor* Instigator)
{
	// This handles cases where the last 'tick' of the effect would happen at almost the same time as the stopping of the action.
	if (GetWorld()->GetTimerManager().GetTimerRemaining(PeriodHandle) < KINDA_SMALL_NUMBER) ExecutePeriodicEffect(Instigator);

	Super::StopAction_Implementation(Instigator);
	GetWorld()->GetTimerManager().ClearTimer(DurationHandle);
	GetWorld()->GetTimerManager().ClearTimer(PeriodHandle);

	if (UCActionComponent* ActionComponent = GetOwningComponent()) ActionComponent->RemoveAction(this, Instigator);
}

void UCActionEffect::ExecutePeriodicEffect_Implementation(AActor* Instigator)
{
	// This implementation will be done by children of this class. This function mustn't be marked with the PURE_VIRTUAL macro as it uses Unreal's Reflection system and has the BlueprintNative flag.
}
