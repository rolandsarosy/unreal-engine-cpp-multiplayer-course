#include "CGameplayFunctionLibrary.h"

#include "Components/CAttributeComponent.h"

bool UCGameplayFunctionLibrary::ApplyDamage(AActor* DamageInstigator, AActor* TargetActor, const float DamageAmount)
{
	if (UCAttributeComponent* AttributeComponent = UCAttributeComponent::GetComponentFrom(TargetActor))
	{
		return AttributeComponent->ApplyHealthChange(DamageInstigator, -DamageAmount);
	}

	return false;
}

bool UCGameplayFunctionLibrary::ApplyDirectionalImpulseDamage(AActor* DamageInstigator, AActor* TargetActor, const float DamageAmount, const FHitResult& HitResult)
{
	if (ApplyDamage(DamageInstigator, TargetActor, DamageAmount))
	{
		if (UPrimitiveComponent* HitComponent = HitResult.GetComponent(); HitComponent && HitComponent->IsSimulatingPhysics(HitResult.BoneName))
		{
			// Direction equals to target minus the origin.
			FVector Direction = HitResult.TraceEnd - HitResult.TraceStart;
			Direction.Normalize();

			HitComponent->AddImpulseAtLocation(Direction * 150000.0f, HitResult.Location, HitResult.BoneName);
		}

		return true;
	}
	return false;
}

bool UCGameplayFunctionLibrary::ApplyHealing(AActor* HealingInstigator, AActor* TargetActor, const float HealAmount)
{
	if (UCAttributeComponent* AttributeComponent = UCAttributeComponent::GetComponentFrom(TargetActor))
	{
		return AttributeComponent->ApplyHealthChange(HealingInstigator, HealAmount);
	}

	return false;
}
