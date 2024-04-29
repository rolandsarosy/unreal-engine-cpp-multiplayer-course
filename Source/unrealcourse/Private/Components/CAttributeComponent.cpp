#include "Components/CAttributeComponent.h"

#include "CGameModeBase.h"

UCAttributeComponent::UCAttributeComponent()
{
	HealthMax = 100;
	HealthCurrent = HealthMax;
}

/**
 * Applies a change to the health value of the actor. Value is clamped between 0 (death) and MaxHealth.
 *
 * @param InstigatorActor Is the actor that causes the health change. Usually the attacker, but in cases of healing, it can be itself or the healing item/actor.
 * @param Delta The amount to change the health value by.
 * @return True if the health change was successfully applied, false otherwise.
 */
bool UCAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, const float Delta)
{
	if (!IsAlive()) return false;
	if (HealthCurrent == HealthMax && Delta >= 0) return false;
	if (!GetOwner()->CanBeDamaged()) return false;

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

	// TODO - Refactor this during assignment 5, to have proper (such as death) callbacks that are included with the AttributeComponent itself.
	if (HealthCurrent == 0.0f)
	{
		ACGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ACGameModeBase>();
		GameMode->OnActorKilled(GetOwner(), InstigatorActor);
	}

	return true;
}

bool UCAttributeComponent::IsAlive() const
{
	return HealthCurrent > 0.0f;
}

bool UCAttributeComponent::Kill(AActor* InstigatorActor)
{
	return ApplyHealthChange(InstigatorActor, -GetHealthMax());
}

float UCAttributeComponent::GetHealthCurrent() const
{
	return HealthCurrent;
}

float UCAttributeComponent::GetHealthMax() const
{
	return HealthMax;
}

UCAttributeComponent* UCAttributeComponent::GetComponentFrom(AActor* FromActor)
{
	if (FromActor) return FromActor->FindComponentByClass<UCAttributeComponent>();
	
	return nullptr;
}
