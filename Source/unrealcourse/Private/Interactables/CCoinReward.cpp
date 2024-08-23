#include "Interactables/CCoinReward.h"

#include "Framework/CPlayerState.h"

#define LOCTEXT_NAMESPACE "InteractableActors"

ACCoinReward::ACCoinReward()
{
	CooldownDuration = 8;
	CoinsRewardAmount = 30;
}

bool ACCoinReward::OnEffectTrigger(APawn* InstigatorPawn)
{
	ACPlayerState* PlayerState = ACPlayerState::GetFromActor(InstigatorPawn);
	if (!PlayerState) return false;

	return PlayerState->AddCoins(this, CoinsRewardAmount);
}


FText ACCoinReward::GetInteractText_Implementation(APawn* InstigatorPawn)
{
	return FText::Format(LOCTEXT("CoinReward_DefaultText", "Grants {0} Coins."), CoinsRewardAmount);
}

#undef LOCTEXT_NAMESPACE
