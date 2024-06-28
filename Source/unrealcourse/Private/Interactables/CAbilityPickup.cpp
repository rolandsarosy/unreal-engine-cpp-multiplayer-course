#include "Interactables/CAbilityPickup.h"

#include "Components/CActionComponent.h"
#include "GAS/CActionAbility.h"

bool ACAbilityPickup::OnEffectTrigger(APawn* InstigatorPawn)
{
	UCActionComponent* ActionComponent = UCActionComponent::GetComponentFrom(InstigatorPawn);
	if (!ensure(ActionComponent)) return false;

	bool HasAbilityAlready = false;

	for (TObjectPtr<UCBaseAction> Action : ActionComponent->CurrentActions)
	{
		if (Action.IsA(GrantsAbility))
		{
			HasAbilityAlready = true;
			break;
		}
	}

	if (HasAbilityAlready) return false;

	ActionComponent->AddAction(GrantsAbility, this);
	return true;
}
