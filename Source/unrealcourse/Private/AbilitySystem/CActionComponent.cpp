#include "AbilitySystem/CActionComponent.h"

#include "AbilitySystem/CAction.h"

UCActionComponent::UCActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCActionComponent::BeginPlay()
{
	Super::BeginPlay();

	for (const TSubclassOf<UCAction> ActionClass : DefaultActions)
	{
		AddAction(ActionClass);
	}
}

void UCActionComponent::TickComponent(const float DeltaTime, const ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	const FString DebugMessage = GetNameSafe(GetOwner()) + " " + ActiveGameplayTags.ToStringSimple();
	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::White, DebugMessage);
}

// ReSharper disable once CppTooWideScopeInitStatement - Results in worse readability
void UCActionComponent::AddAction(const TSubclassOf<UCAction> ActionClass)
{
	if (!ensure(ActionClass)) return;

	UCAction* NewAction = NewObject<UCAction>(this, ActionClass);

	if (ensure(NewAction)) CurrentActions.Add(NewAction);
}

bool UCActionComponent::StartActionByName(AActor* Instigator, const FName ActionName)
{
	for (UCAction* Action : CurrentActions)
	{
		if (Action && Action->ActionName == ActionName && Action->CanStart(Instigator))
		{
			Action->StartAction(Instigator);
			return true;
		}
	}

	return false;
}

bool UCActionComponent::StopActionByName(AActor* Instigator, const FName ActionName)
{
	for (UCAction* Action : CurrentActions)
	{
		if (Action && Action->ActionName == ActionName && Action->IsRunning())
		{
			Action->StopAction(Instigator);
			return true;
		}
	}

	return false;
}
