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
		AddAction(ActionClass, GetOwner()); // At BeginPlay, only default actions are added, so we can assume that the owner is the instigator.
	}
}

void UCActionComponent::TickComponent(const float DeltaTime, const ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	const FString DebugMessage = GetNameSafe(GetOwner()) + " " + ActiveGameplayTags.ToStringSimple();
	// GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::White, DebugMessage);
}

// ReSharper disable once CppTooWideScopeInitStatement - Results in worse readability
void UCActionComponent::AddAction(const TSubclassOf<UCAction> ActionClass, AActor* Instigator)
{
	if (!ensure(ActionClass)) return;

	UCAction* NewAction = NewObject<UCAction>(this, ActionClass);

	if (ensure(NewAction))
	{
		CurrentActions.Add(NewAction);
		if (NewAction->bAutoStart && ensure(NewAction->CanStart(Instigator))) NewAction->StartAction(Instigator);
	}
}

void UCActionComponent::RemoveAction(UCAction* ActionToRemove)
{
	if (!ensure(ActionToRemove && !ActionToRemove->IsRunning())) return;
	CurrentActions.Remove(ActionToRemove);
}

bool UCActionComponent::StartActionByTag(AActor* Instigator, const FGameplayTag Tag)
{
	for (UCAction* Action : CurrentActions)
	{
		if (Action && Action->Tag == Tag && Action->CanStart(Instigator))
		{
			Action->StartAction(Instigator);
			return true;
		}
	}

	return false;
}

bool UCActionComponent::StopActionByTag(AActor* Instigator, FGameplayTag Tag)
{
	for (UCAction* Action : CurrentActions)
	{
		if (Action && Action->Tag == Tag && Action->IsRunning())
		{
			Action->StopAction(Instigator);
			return true;
		}
	}

	return false;
}

void UCActionComponent::AddGameplayTag(const FGameplayTag Tag) { ActiveGameplayTags.AddTag(Tag); }

bool UCActionComponent::RemoveGameplayTag(const FGameplayTag Tag) { return ActiveGameplayTags.RemoveTag(Tag); }
