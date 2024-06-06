#include "GAS/CActionEffect_Thorns.h"

#include "CCharacter.h"
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

	const ACCharacter* Character = Cast<ACCharacter>(Instigator);
	Character->AttributeComponent->OnHealthChanged.AddDynamic(this, &UCActionEffect_Thorns::OnOwnerHealthChanged);
}

void UCActionEffect_Thorns::StopAction_Implementation(AActor* Instigator)
{
	Super::StopAction_Implementation(Instigator);

	const ACCharacter* Character = Cast<ACCharacter>(Instigator);
	Character->AttributeComponent->OnHealthChanged.RemoveDynamic(this, &UCActionEffect_Thorns::OnOwnerHealthChanged);
}

void UCActionEffect_Thorns::OnOwnerHealthChanged(AActor* InstigatorActor, UCAttributeComponent* OwnerComponent, float NewHealth, float Delta)
{
	if (NewHealth == 0) return;
	if (Delta > 0) return;
	if (InstigatorActor == GetOwningComponent()->GetOwner()) return;
	
	const int32 ThornsDamage = abs(round(Delta * (static_cast<float>(ThornsPercentage) / 100.0F)));
	UCGameplayFunctionLibrary::ApplyDamage(GetOwningComponent()->GetOwner(), InstigatorActor, ThornsDamage);
}
