#include "Interactables/CAbilityPickup.h"

#include "Components/CActionComponent.h"
#include "GAS/CActionAbility.h"

#define LOCTEXT_NAMESPACE "InteractableActors"

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

FText ACAbilityPickup::GetInteractText_Implementation(APawn* InstigatorPawn)
{
	FText InteractTextHasAbilityAlready = LOCTEXT("AbilityPickup_HasAbilityAlready", "Dash ability already owned.");
	FText InteractTextDefault = LOCTEXT("AbilityPickup_DefaultText", "Grants the Dash ability.");

	UCActionComponent* ActionComponent = UCActionComponent::GetComponentFrom(InstigatorPawn);
	if (!ensure(ActionComponent)) return LOCTEXT("AbilityPickup_InternalErrorMessage", "Internal error.");;

	bool bHasAbilityAlready = false;
	for (TObjectPtr<UCBaseAction> Action : ActionComponent->CurrentActions)
	{
		if (Action.IsA(GrantsAbility))
		{
			bHasAbilityAlready = true;
			break;
		}
	}

	return bHasAbilityAlready ? InteractTextHasAbilityAlready : InteractTextDefault;
}

#undef LOCTEXT_NAMESPACE
