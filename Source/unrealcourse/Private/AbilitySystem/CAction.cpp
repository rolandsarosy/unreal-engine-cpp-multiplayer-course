#include "AbilitySystem/CAction.h"

#include "AbilitySystem/CActionComponent.h"

void UCAction::StartAction_Implementation(AActor* Instigator)
{
	GetOwningComponent()->ActiveGameplayTags.AppendTags(GrantsTags);
	bIsRunning = true;
}

void UCAction::StopAction_Implementation(AActor* Instigator)
{
	ensureAlways(bIsRunning); // Sanity check to notify developers that an issue got introduced as an action that isn't running, could never be stopped.

	GetOwningComponent()->ActiveGameplayTags.RemoveTags(GrantsTags);
	bIsRunning = false;
}

bool UCAction::CanStart_Implementation(AActor* Instigator)
{
	return !bIsRunning && !GetOwningComponent()->ActiveGameplayTags.HasAny(BlockedTags);
}

UWorld* UCAction::GetWorld() const
{
	// Outer is set when creating a new action via NewObject<T>.
	if (const UActorComponent* ActorComponent = Cast<UActorComponent>(GetOuter()))
	{
		return ActorComponent->GetWorld();
	}
	return nullptr;
}

UCActionComponent* UCAction::GetOwningComponent() const
{
	return Cast<UCActionComponent>(GetOuter());
}

bool UCAction::IsRunning() const
{
	return bIsRunning;
}
