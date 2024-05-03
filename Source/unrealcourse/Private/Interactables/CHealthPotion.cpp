#include "Interactables/CHealthPotion.h"

#include "CGameplayFunctionLibrary.h"
#include "CPlayerState.h"

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

	const bool HasEnoughCoins = PlayerState->GetCoinsAmount() >= CoinsCost;
	if (!HasEnoughCoins) return false;

	const bool DidHealSuccessfully = UCGameplayFunctionLibrary::ApplyHealing(this, InstigatorPawn, HealthRestoreAmount);
	if (!DidHealSuccessfully) return false;

	if (HasEnoughCoins && DidHealSuccessfully) return PlayerState->RemoveCoins(this, CoinsCost);

	return false;
}
