#include "GAS/CActionEffect_Thorns.h"

#include "CGameplayFunctionLibrary.h"
#include "Components/CActionComponent.h"
#include "Components/CAttributeComponent.h"

UCActionEffect_Thorns::UCActionEffect_Thorns()
{
	Duration = 0.0f;
	ThornsPercentage = 10;
}

void UCActionEffect_Thorns::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	if (UCAttributeComponent* AttributeComponent = UCAttributeComponent::GetComponentFrom(Instigator))
	{
		AttributeComponent->OnHealthChanged.AddDynamic(this, &UCActionEffect_Thorns::OnOwnerHealthChanged);
	}
}

void UCActionEffect_Thorns::StopAction_Implementation(AActor* Instigator)
{
	Super::StopAction_Implementation(Instigator);

	if (UCAttributeComponent* AttributeComponent = UCAttributeComponent::GetComponentFrom(Instigator))
	{
		AttributeComponent->OnHealthChanged.RemoveDynamic(this, &UCActionEffect_Thorns::OnOwnerHealthChanged);
	}
}

void UCActionEffect_Thorns::OnOwnerHealthChanged(AActor* InstigatorActor, UCAttributeComponent* OwnerComponent, float NewHealth, float Delta)
{
	if (NewHealth == 0) return;
	if (Delta > 0) return;
	if (InstigatorActor == GetOwningComponent()->GetOwner()) return;

	const int32 ThornsDamage = abs(round(Delta * (static_cast<float>(ThornsPercentage) / 100.0F)));

	if (ThornsDamage == 0) return;

	UCGameplayFunctionLibrary::ApplyDamage(GetOwningComponent()->GetOwner(), InstigatorActor, ThornsDamage);
}
