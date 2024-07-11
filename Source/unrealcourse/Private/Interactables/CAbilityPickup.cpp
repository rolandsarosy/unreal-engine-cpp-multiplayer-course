#include "Interactables/CAbilityPickup.h"

#include "Components/CActionComponent.h"
#include "GAS/CActionAbility.h"

bool ACAbilityPickup::OnEffectTrigger(APawn* InstigatorPawn)
{
	UCActionComponent* ActionComponent = UCActionComponent::GetComponentFrom(InstigatorPawn);
	if (!ensure(ActionComponent)) return false;

	bool bHasAbilityAlready = false;

	for (TObjectPtr<UCBaseAction> Action : ActionComponent->CurrentActions)
	{
		if (Action.IsA(GrantsAbility))
		{
			bHasAbilityAlready = true;
			break;
		}
	}

	if (bHasAbilityAlready) return false;

	ActionComponent->AddAction(GrantsAbility, this);
	return true;
}
