#include "Components/CAttributeComponent.h"

#include "CGameModeBase.h"

static TAutoConsoleVariable CVarDamageMultiplier(TEXT("course.DamageMultiplier"), 1.0f, TEXT("Global damage multiplier for the AttributeComponent."), ECVF_Cheat);
static TAutoConsoleVariable CVarHealingMultiplier(TEXT("course.HealingMultiplier"), 1.0f, TEXT("Global healing multiplier for the AttributeComponent."), ECVF_Cheat);
static TAutoConsoleVariable CVarRageGainMultiplier(TEXT("course.RageGainMultiplier"), 1.0f, TEXT("Global Rage gain multiplier for the AttributeComponent."), ECVF_Cheat);
static TAutoConsoleVariable CVarRageCostMultiplier(TEXT("course.RageCostMultiplier"), 1.0f, TEXT("Global Rage cost multiplier for the AttributeComponent."), ECVF_Cheat);

UCAttributeComponent::UCAttributeComponent()
{
	HealthMax = 100;
	HealthCurrent = HealthMax;
	IsRageEnabled = false;
	RageGainPercentage = 0;
	RageCurrent = 0;
	RageMax = 0;
}

/**
 * Applies a change to the health value of the actor. Value is clamped between 0 (death) and MaxHealth.
 *
 * @param InstigatorActor Is the actor that causes the health change. Usually the attacker, but in cases of healing, it can be itself or the healing item/actor.
 * @param Delta The amount to change the health value by.
 * @return True if the health change was successfully applied, false otherwise.
 */
bool UCAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	if (!IsAlive()) return false;
	if (HealthCurrent == HealthMax && Delta >= 0) return false;
	if (!GetOwner()->CanBeDamaged() && Delta < 0) return false;

	if (Delta > 0) Delta *= CVarHealingMultiplier.GetValueOnGameThread();
	if (Delta < 0) Delta *= CVarDamageMultiplier.GetValueOnGameThread();

	if (const float ProposedHealth = HealthCurrent + Delta; ProposedHealth < 0.0f) // Cases where the result would be overkill
	{
		HealthCurrent = 0.0f;
		OnHealthChanged.Broadcast(InstigatorActor, this, HealthCurrent, Delta - ProposedHealth);
	}
	else if (ProposedHealth > HealthMax) // Cases where the result would be overheal
	{
		HealthCurrent = HealthMax;
		OnHealthChanged.Broadcast(InstigatorActor, this, HealthCurrent, HealthMax - ProposedHealth + Delta);
	}
	else // Cases where the result is within 0 and max health.
	{
		HealthCurrent = ProposedHealth;
		OnHealthChanged.Broadcast(InstigatorActor, this, HealthCurrent, Delta);

		if (Delta < 0) ApplyRageChange(InstigatorActor, std::abs(Delta) * (static_cast<float>(RageGainPercentage) / 100.0F));
	}

	if (!IsAlive())
	{
		OnDeath.Broadcast(InstigatorActor, this);

		// I greatly dislike this hard-coupled nightmare of a statement, but this is what was done in class. TODO: Use something like a GameplayMessage or maybe a delegate of sorts here.
		GetWorld()->GetAuthGameMode<ACGameModeBase>()->OnActorKilled(GetOwner(), InstigatorActor);
	}

	return true;
}

bool UCAttributeComponent::ApplyRageChange(AActor* InstigatorActor, float Delta)
{
	if (!IsRageEnabled) return false;
	if (!IsAlive()) return false;
	if (RageCurrent == 0 && Delta < 0) return false;
	if (RageCurrent == RageMax) return false;

	if (Delta > 0) Delta *= CVarRageGainMultiplier.GetValueOnGameThread();
	if (Delta < 0) Delta *= CVarRageCostMultiplier.GetValueOnGameThread();

	if (const float ProposedRage = RageCurrent + Delta; ProposedRage > RageMax) // Cases where the result would be over the maximum amount of Rage.
	{
		RageCurrent = RageMax;
		OnRageChanged.Broadcast(InstigatorActor, this, RageCurrent, RageMax - (ProposedRage - Delta));
	}
	else if (ProposedRage < 0) // Cases where the result would be below 0
	{
		RageCurrent = 0;
		OnRageChanged.Broadcast(InstigatorActor, this, RageCurrent, Delta - ProposedRage);
	}
	else // Cases where the result is between 0 and the maximum amount of Rage
	{
		RageCurrent = ProposedRage;
		OnRageChanged.Broadcast(InstigatorActor, this, RageCurrent, Delta);
	}

	return true;
}

bool UCAttributeComponent::IsAlive() const { return HealthCurrent > 0.0f; }

bool UCAttributeComponent::KillOwner(AActor* InstigatorActor) { return ApplyHealthChange(InstigatorActor, -GetHealthMax()); }

float UCAttributeComponent::GetHealthCurrent() const { return HealthCurrent; }

float UCAttributeComponent::GetHealthMax() const { return HealthMax; }

float UCAttributeComponent::GetRageCurrent() const { return RageCurrent; }

float UCAttributeComponent::GetRageMax() const { return RageMax; }

UCAttributeComponent* UCAttributeComponent::GetComponentFrom(AActor* FromActor) { return FromActor ? FromActor->FindComponentByClass<UCAttributeComponent>() : nullptr; }
