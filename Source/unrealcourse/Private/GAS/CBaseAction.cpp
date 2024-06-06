#include "GAS/CBaseAction.h"

#include "Components/CActionComponent.h"

void UCBaseAction::StartAction_Implementation(AActor* Instigator)
{
	GetOwningComponent()->ActiveGameplayTags.AppendTags(GrantsTags);
	GetOwningComponent()->OnActionStarted.Broadcast(this, Instigator);
	bIsRunning = true;
}

void UCBaseAction::StopAction_Implementation(AActor* Instigator)
{
	ensureAlways(bIsRunning); // Sanity check to notify developers that an issue got introduced as an action that isn't running, could never be stopped.

	GetOwningComponent()->ActiveGameplayTags.RemoveTags(GrantsTags);
	GetOwningComponent()->OnActionStopped.Broadcast(this, Instigator);
	bIsRunning = false;
}

bool UCBaseAction::CanStart_Implementation(AActor* Instigator)
{
	return !bIsRunning && !GetOwningComponent()->ActiveGameplayTags.HasAny(BlockedTags);
}

UWorld* UCBaseAction::GetWorld() const
{
	// Outer is set when creating a new action via NewObject<T>.
	if (const UActorComponent* ActorComponent = Cast<UActorComponent>(GetOuter()))
	{
		return ActorComponent->GetWorld();
	}
	return nullptr;
}

UCActionComponent* UCBaseAction::GetOwningComponent() const
{
	return Cast<UCActionComponent>(GetOuter());
}

bool UCBaseAction::IsRunning() const
{
	return bIsRunning;
}
