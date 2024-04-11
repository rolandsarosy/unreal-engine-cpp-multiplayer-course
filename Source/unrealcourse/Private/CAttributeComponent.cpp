#include "CAttributeComponent.h"

UCAttributeComponent::UCAttributeComponent()
{
	HealthMax = 100;
	HealthCurrent = HealthMax;
}

// The original assignment was to clamp between 0-MaxHealth with FMath::Clamp. I believe this solution is better, so I've implemented this instead.
bool UCAttributeComponent::ApplyHealthChange(const float Delta)
{
	if (!IsAlive()) return false;

	if (const float ProposedHealth = HealthCurrent + Delta; ProposedHealth < 0.0f)
	{
		HealthCurrent = 0.0f;
		OnHealthChanged.Broadcast(nullptr, this, HealthCurrent, Delta - ProposedHealth);
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
