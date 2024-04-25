#include "Interactables/CHealthPotion.h"

#include "Components/CAttributeComponent.h"

ACHealthPotion::ACHealthPotion()
{
	CooldownDuration = 10.0f;
	HealthRestoreAmount = 20;
}

bool ACHealthPotion::OnEffectTrigger(APawn* InstigatorPawn)
{
	if (UCAttributeComponent* AttributeComponent = UCAttributeComponent::GetComponentFrom(InstigatorPawn))
	{
		return AttributeComponent->ApplyHealthChange(this, HealthRestoreAmount);
	}

	return false;
}
