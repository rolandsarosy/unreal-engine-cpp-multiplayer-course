#include "Components/CAttributeComponent.h"

UCAttributeComponent::UCAttributeComponent()
{
	HealthMax = 100;
	HealthCurrent = HealthMax;
}

// The original assignment was to clamp between 0-MaxHealth with FMath::Clamp. I believe this solution is better, so I've implemented this instead.
bool UCAttributeComponent::ApplyHealthChange(const float Delta)
{
	if (!IsAlive()) return false;
	if (HealthCurrent == HealthMax && Delta >= 0) return false;

	if (const float ProposedHealth = HealthCurrent + Delta; ProposedHealth < 0.0f) // Cases where the result would be overkill
	{
		HealthCurrent = 0.0f;
		OnHealthChanged.Broadcast(nullptr, this, HealthCurrent, Delta - ProposedHealth);
	}
	else if (ProposedHealth > HealthMax) // Cases where the result would be overheal
	{
		HealthCurrent = HealthMax;
		OnHealthChanged.Broadcast(nullptr, this, HealthCurrent, HealthMax - ProposedHealth + Delta);
	}
	else
	{
		HealthCurrent = ProposedHealth;
		OnHealthChanged.Broadcast(nullptr, this, HealthCurrent, Delta);
	}

	return true;
}

bool UCAttributeComponent::IsAlive() const
{
	return HealthCurrent > 0.0f;
}
