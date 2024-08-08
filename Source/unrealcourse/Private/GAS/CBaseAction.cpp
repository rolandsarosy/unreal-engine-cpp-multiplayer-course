#include "GAS/CBaseAction.h"

#include "Components/CActionComponent.h"
#include "Net/UnrealNetwork.h"

void UCBaseAction::StartAction_Implementation(AActor* Instigator)
{
	// LogOnScreen(this, FString::Printf(TEXT("Started: %s"), *Tag.ToString()), FColor::Green, 1.0F); // Should be enabled for Action debugging.

	GetOwningComponent()->ActiveGameplayTags.AppendTags(GrantsTags);
	GetOwningComponent()->OnActionStarted.Broadcast(this, Instigator);
	ReplicationData.Instigator = Instigator;
	ReplicationData.bIsRunning = true;

	if (GetOwningComponent()->GetOwner()->HasAuthority()) TimeStarted = GetWorld()->TimeSeconds; // TimeStarted should only come from a trusted source.
}

void UCBaseAction::StopAction_Implementation(AActor* Instigator)
{
	// LogOnScreen(this, FString::Printf(TEXT("Stopped: %s"), *Tag.ToString()), FColor::Black, 1.0F); // Should be enabled for Action debugging.

	GetOwningComponent()->ActiveGameplayTags.RemoveTags(GrantsTags);
	GetOwningComponent()->OnActionStopped.Broadcast(this, Instigator);
	ReplicationData.Instigator = Instigator;
	ReplicationData.bIsRunning = false;
}

bool UCBaseAction::CanStart_Implementation(AActor* Instigator)
{
	return !ReplicationData.bIsRunning && !GetOwningComponent()->ActiveGameplayTags.HasAny(BlockedTags);
}

UWorld* UCBaseAction::GetWorld() const
{
	// Outer is set when creating a new action via NewObject<T>.
	if (const UCActionComponent* ActorComponent = Cast<UCActionComponent>(GetOuter()))
	{
		return ActorComponent->GetWorld();
	}
	return nullptr;
}

void UCBaseAction::OnRep_ReplicationData()
{
	if (IsRunning())
	{
		StartAction(ReplicationData.Instigator);
	}
	else
	{
		StopAction(ReplicationData.Instigator);
	}
}

bool UCBaseAction::IsRunning() const { return ReplicationData.bIsRunning; }

UCActionComponent* UCBaseAction::GetOwningComponent() const { return Cast<UCActionComponent>(GetOuter()); }

void UCBaseAction::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCBaseAction, ReplicationData);
	DOREPLIFETIME(UCBaseAction, TimeStarted);
}
