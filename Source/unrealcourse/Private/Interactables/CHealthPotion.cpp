#include "Interactables/CHealthPotion.h"

#include "CGameplayFunctionLibrary.h"
#include "Framework/CPlayerState.h"

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
