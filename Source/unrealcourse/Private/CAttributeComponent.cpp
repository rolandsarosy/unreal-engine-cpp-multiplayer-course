#include "CAttributeComponent.h"

UCAttributeComponent::UCAttributeComponent()
{
	Health = 100;
}

bool UCAttributeComponent::ApplyHealthChange(float Delta)
{
	Health += Delta;
	OnHealthChanged.Broadcast(nullptr, this, Health, Delta);
	
	return true; // Later on this will return whether a change had been made or not.
}

bool UCAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}
