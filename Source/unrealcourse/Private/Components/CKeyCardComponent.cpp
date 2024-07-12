#include "Components/CKeyCardComponent.h"

#include "Net/UnrealNetwork.h"
#include "unrealcourse/unrealcourse.h"

UCKeyCardComponent::UCKeyCardComponent()
{
	SetIsReplicatedByDefault(true);

#if !UE_BUILD_SHIPPING
	PrimaryComponentTick.bCanEverTick = false; // Enable this only for debugging and such.
#endif
}

void UCKeyCardComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner()->HasAuthority() && !DefaultKeycardGameplayTags.IsEmpty()) // Server only
	{
		for (const FGameplayTag Tag : DefaultKeycardGameplayTags) { AddKeycardGameplayTag(Tag); }
	}
}

/**
 * @brief Called when the CurrentKeycardGameplayTags container is replicated.
 *
 * Compares the previous container with the current container to determine if any tags have been added or removed.
 *
 * @note Notifies observers locally.
 */
void UCKeyCardComponent::OnRep_CurrentKeycardGameplayTags()
{
	if (PreviousKeycardGameplayTags.Num() == CurrentKeycardGameplayTags.Num()) return;
	if (!Cast<APawn>(GetOwner())->IsLocallyControlled()) return;

	if (PreviousKeycardGameplayTags.Num() < CurrentKeycardGameplayTags.Num()) // Assume an Action got added.
	{
		for (const auto& Tag : CurrentKeycardGameplayTags)
		{
			if (!PreviousKeycardGameplayTags.HasTag(Tag)) { OnKeycardGameplayTagAdded.Broadcast(Tag); }
		}
	}

	if (PreviousKeycardGameplayTags.Num() > CurrentKeycardGameplayTags.Num()) // Assume an Action got removed.
	{
		for (const auto& Tag : PreviousKeycardGameplayTags)
		{
			if (!CurrentKeycardGameplayTags.HasTag(Tag)) { OnKeycardGameplayTagRemoved.Broadcast(Tag); }
		}
	}
	PreviousKeycardGameplayTags = CurrentKeycardGameplayTags;
}

void UCKeyCardComponent::AddKeycardGameplayTag_Implementation(const FGameplayTag Tag)
{
	CurrentKeycardGameplayTags.AddTag(Tag);
	OnRep_CurrentKeycardGameplayTags();
}

void UCKeyCardComponent::RemoveKeycardGameplayTag_Implementation(const FGameplayTag Tag)
{
	if (CurrentKeycardGameplayTags.RemoveTag(Tag)) OnRep_CurrentKeycardGameplayTags();
}

void UCKeyCardComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

#if !UE_BUILD_SHIPPING
	FString Msg;
	Msg.Append("Tags: ");

	for (const FGameplayTag Tag : CurrentKeycardGameplayTags)
	{
		Msg.Append(FString::Printf(TEXT("[%s], "), *Tag.GetTagName().ToString()));
	}

	if (Msg.Len() > 0)
	{
		Msg.RemoveAt(Msg.Len() - 2, 2);
	}

	LogOnScreen(this, Msg, FColor::Blue, 0.0F);
#endif
}

UCKeyCardComponent* UCKeyCardComponent::GetComponentFrom(AActor* FromActor) { return FromActor ? FromActor->FindComponentByClass<UCKeyCardComponent>() : nullptr; }

void UCKeyCardComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCKeyCardComponent, CurrentKeycardGameplayTags);
}
