#include "Components/CActionComponent.h"

#include "Engine/ActorChannel.h"
#include "GAS/CBaseAction.h"
#include "Net/UnrealNetwork.h"
#include "unrealcourse/unrealcourse.h"

DECLARE_CYCLE_STAT(TEXT("AddAction"), STAT_AddAction, STATGROUP_UNREALCOURSE)
DECLARE_CYCLE_STAT(TEXT("StartActionByTag"), STAT_StartActionByTag, STATGROUP_UNREALCOURSE)
DECLARE_CYCLE_STAT(TEXT("StopActionByTag"), STAT_StopActionByTag, STATGROUP_UNREALCOURSE)

UCActionComponent::UCActionComponent()
{
	SetIsReplicatedByDefault(true);

#if !UE_BUILD_SHIPPING
	PrimaryComponentTick.bCanEverTick = false; // Enable this only for debugging and such.
#endif
}

/**
 * @brief Called when the CurrentActions array is replicated.
 *
 * Compares the previous actions array with the current actions array to determine if any actions have been added or removed.
 *
 * @note Notifies observers locally.
 */
void UCActionComponent::OnRep_CurrentActions(TArray<UCBaseAction*> PreviousActions)
{
	if (PreviousActions.Num() == CurrentActions.Num()) return;

	if (PreviousActions.Num() < CurrentActions.Num()) // Assume an Action got added.
	{
		for (const auto& Action : CurrentActions)
		{
			if (!PreviousActions.Contains(Action)) { OnActionAdded.Broadcast(Action); }
		}
	}

	if (PreviousActions.Num() > CurrentActions.Num()) // Assume an Action got removed.
	{
		for (const auto& Action : PreviousActions)
		{
			if (!CurrentActions.Contains(Action)) { OnActionRemoved.Broadcast(Action); }
		}
	}
}

void UCActionComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner()->HasAuthority()) // Server only
	{
		// At BeginPlay, only default actions are added, so we can assume that the owner is the instigator.
		for (const TSubclassOf<UCBaseAction> ActionClass : DefaultActions) { AddAction(ActionClass, GetOwner()); }
	}
}

// Use this only for debugging and such. Make sure this is disabled in a final build.
void UCActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

#if !UE_BUILD_SHIPPING
	for (const UCBaseAction* Action : CurrentActions)
	{
		if (!Action) return;
		const FColor TextColor = Action->IsRunning() ? FColor::Blue : FColor::White;
		FString ActionMSg = FString::Printf(TEXT("[%s] Action: %s"), *GetNameSafe(GetOwner()), *Action->Tag.ToString());
		LogOnScreen(this, ActionMSg, TextColor, 0.0f);
	}
#endif
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
	SCOPE_CYCLE_COUNTER(STAT_AddAction)

	if (!ensure(ActionClass)) return;
	if (!ensure(GetOwner()->HasAuthority()))
	{
		UE_LOG(LogTemp, Warning, TEXT("A non-authoritave client attempted to add an action. [Class: %s]"), *GetNameSafe(ActionClass));
		return;
	}
	
	if (UCBaseAction* NewAction = NewObject<UCBaseAction>(this, ActionClass); ensure(NewAction))
	{
		const TArray<UCBaseAction*> PreviousStateActionList = CurrentActions; // Create a separate list for calling OnRep on server as OnRep does not get called automatically there.
		CurrentActions.Add(NewAction);
		OnRep_CurrentActions(PreviousStateActionList);

		if (NewAction->bAutoStart && NewAction->CanStart(Instigator))
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
	SCOPE_CYCLE_COUNTER(STAT_StartActionByTag)

	for (UCBaseAction* Action : CurrentActions)
	{
		if (Action && Action->Tag == Tag && Action->CanStart(Instigator))
		{
			TRACE_BOOKMARK(TEXT("StartAction::%s"), *Action->Tag.ToString());
			if (!GetOwner()->HasAuthority()) ServerStartAction(Instigator, Tag);
			Action->StartAction(Instigator);
			return true;
		}
	}

	return false;
}

void UCActionComponent::ServerStartAction_Implementation(AActor* Instigator, const FGameplayTag Tag)
{
	StartActionByTag(Instigator, Tag);
}

/**
 * Stops the action with the specified tag for the given Instigator actor.
 *
 * @param Instigator The actor that initiated the action.
 * @param Tag The gameplay tag of the action to start.
 *
 * @return True if the action was successfully started, false otherwise.
 */
bool UCActionComponent::StopActionByTag(AActor* Instigator, const FGameplayTag Tag)
{
	SCOPE_CYCLE_COUNTER(STAT_StopActionByTag)
	
	for (UCBaseAction* Action : CurrentActions)
	{
		if (Action && Action->Tag == Tag && Action->IsRunning())
		{
			if (!GetOwner()->HasAuthority()) ServerStopAction(Instigator, Tag);
			Action->StopAction(Instigator);
			return true;
		}
	}

	return false;
}

void UCActionComponent::ServerStopAction_Implementation(AActor* Instigator, const FGameplayTag Tag)
{
	StopActionByTag(Instigator, Tag);
}

/**
 * Returns the UCActionComponent attached to an AActor. If the AActor does not have a UCActionComponent, it returns nullptr.
 *
 * @param FromActor The actor to get the UCActionComponent from.
 * 
 * @return UCActionComponent* The UCActionComponent attached to the FromActor, or nullptr if it does not have a UCActionComponent.
 */
UCActionComponent* UCActionComponent::GetComponentFrom(AActor* FromActor) { return FromActor ? FromActor->FindComponentByClass<UCActionComponent>() : nullptr; }

bool UCActionComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool bChangeOccured = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	for (UCBaseAction* Action : CurrentActions)
	{
		if (Action) bChangeOccured |= Channel->ReplicateSubobject(Action, *Bunch, *RepFlags);
	}
	return bChangeOccured;
}

void UCActionComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	for (TArray<UCBaseAction*> ActionsCopy = CurrentActions; UCBaseAction* Action : ActionsCopy)
	{
		if (Action && Action->IsRunning()) { Action->StopAction(GetOwner()); }
	}

	Super::EndPlay(EndPlayReason);
}

void UCActionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCActionComponent, CurrentActions);
}
