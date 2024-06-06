#include "Components/CActionComponent.h"
#include "GAS/CBaseAction.h"

void UCActionComponent::BeginPlay()
{
	Super::BeginPlay();

	for (const TSubclassOf<UCBaseAction> ActionClass : DefaultActions)
	{
		AddAction(ActionClass, GetOwner()); // At BeginPlay, only default actions are added, so we can assume that the owner is the instigator.
	}
}

/**
 * Adds a new action to this ActionComponent. If the action has auto start set to true, it'll immediately be started.
 *
 * @note Auto start should usually be used by ActionEffects.
 *
 * @param ActionClass The class of the action to add.
 * @param Instigator The actor that initiated the action.
 */
void UCActionComponent::AddAction(const TSubclassOf<UCBaseAction> ActionClass, AActor* Instigator)
{
	if (!ensure(ActionClass)) return;

	UCBaseAction* NewAction = NewObject<UCBaseAction>(this, ActionClass);
	if (ensure(NewAction))
	{
		CurrentActions.Add(NewAction);
		OnActionAdded.Broadcast(NewAction, Instigator);

		if (NewAction->bAutoStart && ensure(NewAction->CanStart(Instigator)))
		{
			NewAction->StartAction(Instigator);
		}
	}
}

/**
 * Removes the specified action from the ActionComponent.
 *
 * @param ActionToRemove The action to remove.
 * @param Instigator
 */
void UCActionComponent::RemoveAction(UCBaseAction* ActionToRemove, AActor* Instigator)
{
	if (!ensure(ActionToRemove && !ActionToRemove->IsRunning())) return;
	CurrentActions.Remove(ActionToRemove);
	OnActionRemoved.Broadcast(ActionToRemove, Instigator);
}

/**
 * Starts the action with the specified tag for the given Instigator actor.
 *
 * @param Instigator The actor that initiated the action.
 * @param Tag The gameplay tag of the action to start.
 *
 * @return True if the action was successfully started, false otherwise.
 */
bool UCActionComponent::StartActionByTag(AActor* Instigator, const FGameplayTag Tag)
{
	for (UCBaseAction* Action : CurrentActions)
	{
		if (Action && Action->Tag == Tag && Action->CanStart(Instigator))
		{
			Action->StartAction(Instigator);
			return true;
		}
	}

	return false;
}

/**
 * Stops the action with the specified tag for the given Instigator actor.
 *
 * @param Instigator The actor that initiated the action.
 * @param Tag The gameplay tag of the action to start.
 *
 * @return True if the action was successfully started, false otherwise.
 */
bool UCActionComponent::StopActionByTag(AActor* Instigator, FGameplayTag Tag)
{
	for (UCBaseAction* Action : CurrentActions)
	{
		if (Action && Action->Tag == Tag && Action->IsRunning())
		{
			Action->StopAction(Instigator);
			return true;
		}
	}

	return false;
}

/**
 * Adds a gameplay tag to the ActiveGameplayTags container.
 *
 * @param Tag The gameplay tag to add.
 *
 * @note Should be used for adding/removing keycard-style GameplayTags.
 */
void UCActionComponent::AddGameplayTag(const FGameplayTag Tag)
{
	ActiveGameplayTags.AddTag(Tag);
	OnGameplayTagAdded.Broadcast(Tag);
}

/**
 * Removes a gameplay tag from the ActiveGameplayTags container.
 *
 * @param Tag The gameplay tag to remove.
 *
 * @note Should be used for adding/removing keycard-style GameplayTags.
 */
bool UCActionComponent::RemoveGameplayTag(const FGameplayTag Tag)
{
	const bool IsSuccess = ActiveGameplayTags.RemoveTag(Tag);
	if (IsSuccess) OnGameplayTagRemoved.Broadcast(Tag);
	
	return IsSuccess;
}

/**
 * Returns the UCActionComponent attached to an AActor. If the AActor does not have a UCActionComponent, it returns nullptr.
 *
 * @param FromActor The actor to get the UCActionComponent from.
 * 
 * @return UCActionComponent* The UCActionComponent attached to the FromActor, or nullptr if it does not have a UCActionComponent.
 */
UCActionComponent* UCActionComponent::GetComponentFrom(AActor* FromActor) { return FromActor ? FromActor->FindComponentByClass<UCActionComponent>() : nullptr; }
