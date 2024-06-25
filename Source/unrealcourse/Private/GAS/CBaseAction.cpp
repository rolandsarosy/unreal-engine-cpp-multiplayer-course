#include "GAS/CBaseAction.h"

#include "Components/CActionComponent.h"
#include "Net/UnrealNetwork.h"
#include "unrealcourse/unrealcourse.h"

void UCBaseAction::Initialize(UCActionComponent* NewActionComponent)
{
	OwningActionComponent = NewActionComponent;
}

void UCBaseAction::StartAction_Implementation(AActor* Instigator)
{
	LogOnScreen(this, FString::Printf(TEXT("Started: %s"), *Tag.ToString()), FColor::Green, 1.5F);

	GetOwningComponent()->ActiveGameplayTags.AppendTags(GrantsTags);
	GetOwningComponent()->OnActionStarted.Broadcast(this, Instigator);
	bIsRunning = true;
}

void UCBaseAction::StopAction_Implementation(AActor* Instigator)
{
	LogOnScreen(this, FString::Printf(TEXT("Stopped: %s"), *Tag.ToString()), FColor::White, 1.5F);

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
	if (const AActor* Actor = Cast<AActor>(GetOuter()))
	{
		return Actor->GetWorld();
	}
	return nullptr;
}

void UCBaseAction::OnRep_IsRunning()
{
	if (IsRunning())
	{
		StartAction(nullptr);
	}
	else
	{
		StopAction(nullptr);
	}
}

bool UCBaseAction::IsRunning() const { return bIsRunning; }

UCActionComponent* UCBaseAction::GetOwningComponent() const { return OwningActionComponent; }

void UCBaseAction::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCBaseAction, bIsRunning);
	DOREPLIFETIME(UCBaseAction, OwningActionComponent);
}
