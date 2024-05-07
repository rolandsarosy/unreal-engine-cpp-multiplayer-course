#include "AbilitySystem/CAction.h"

void UCAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Starting Action (%s)"), *GetNameSafe(this));
}

void UCAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Stopping Action (%s)"), *GetNameSafe(this));
}

// I don't know about this... This was written in class but this seems pretty hacky, tightly-coupled and quite unsafe. 
UWorld* UCAction::GetWorld() const
{
	// Outer is set when creating a new action via NewObject<T>.
	if (const UActorComponent* ActorComponent = Cast<UActorComponent>(GetOuter()))
	{
		return ActorComponent->GetWorld();
	}
	return nullptr;
}
