#include "AbilitySystem/CActionComponent.h"

#include "AbilitySystem/CAction.h"

void UCActionComponent::BeginPlay()
{
	Super::BeginPlay();

	for (const TSubclassOf<UCAction> ActionClass : DefaultActions)
	{
		AddAction(ActionClass);
	}
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
		if (Action && Action->ActionName == ActionName)
		{
			Action->StartAction(Instigator);
			return true; // Ideally this returns from the Action's function itself.
		}
	}

	return false;
}

bool UCActionComponent::StopActionByName(AActor* Instigator, FName ActionName)
{
	for (UCAction* Action : CurrentActions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			Action->StopAction(Instigator);
			return true; // Ideally this returns from the Action's function itself.
		}
	}

	return false;
}
