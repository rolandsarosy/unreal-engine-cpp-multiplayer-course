#include "Interactables/CCoinReward.h"

#include "CPlayerState.h"

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
