#include "Interactables/CHealthPotion.h"

#include "CGameplayFunctionLibrary.h"
#include "Components/CAttributeComponent.h"
#include "Framework/CPlayerState.h"

#define LOCTEXT_NAMESPACE "InteractableActors"

ACHealthPotion::ACHealthPotion()
{
	CooldownDuration = 10.0f;
	HealthRestoreAmount = 20;
	CoinsCost = 50;
}

bool ACHealthPotion::OnEffectTrigger(APawn* InstigatorPawn)
{
	ACPlayerState* PlayerState = ACPlayerState::GetFromActor(InstigatorPawn);
	if (!PlayerState) return false;

	const bool bHasEnoughCoins = PlayerState->GetCoinsAmount() >= CoinsCost;
	if (!bHasEnoughCoins) return false;

	const bool bDidHealSuccessfully = UCGameplayFunctionLibrary::ApplyHealing(this, InstigatorPawn, HealthRestoreAmount);
	if (!bDidHealSuccessfully) return false;

	if (bHasEnoughCoins && bDidHealSuccessfully) return PlayerState->RemoveCoins(this, CoinsCost);

	return false;
}

FText ACHealthPotion::GetInteractText_Implementation(APawn* InstigatorPawn)
{
	const UCAttributeComponent* AttributeComponent = UCAttributeComponent::GetComponentFrom(InstigatorPawn);
	const ACPlayerState* PlayerState = ACPlayerState::GetFromActor(InstigatorPawn);

	if (!AttributeComponent || !PlayerState) return LOCTEXT("HealthPotion_InternalErrorMessage", "Internal error.");

	const float MissingHealth = AttributeComponent->GetHealthMax() - AttributeComponent->GetHealthCurrent();
	const float PotentialHealthRestoreAmount = MissingHealth >= HealthRestoreAmount ? HealthRestoreAmount : MissingHealth;

	FText InteractTextMaxHealth = LOCTEXT("HealthPotion_MaxHealth", "Already at max health.");
	FText InteractTextCannotAfford = FText::Format(LOCTEXT("HealthPotion_CannotAfford", "Not enough Coins. Costs {0}."), CoinsCost);
	FText InteractTextDefault = FText::Format(LOCTEXT("HealthPotion_DefaultText", "Restores {0} Health for {1} Coins."), PotentialHealthRestoreAmount, CoinsCost);

	if (PlayerState->GetCoinsAmount() < CoinsCost) return InteractTextCannotAfford;
	if (PotentialHealthRestoreAmount == 0) return InteractTextMaxHealth;

	return InteractTextDefault;
}

#undef LOCTEXT_NAMESPACE
