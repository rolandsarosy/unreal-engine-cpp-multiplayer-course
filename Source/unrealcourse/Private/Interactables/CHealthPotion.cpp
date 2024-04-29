#include "Interactables/CHealthPotion.h"

#include "CGameplayFunctionLibrary.h"

ACHealthPotion::ACHealthPotion()
{
	CooldownDuration = 10.0f;
	HealthRestoreAmount = 20;
}

bool ACHealthPotion::OnEffectTrigger(APawn* InstigatorPawn)
{
	return UCGameplayFunctionLibrary::ApplyHealing(this, InstigatorPawn, HealthRestoreAmount);
}
