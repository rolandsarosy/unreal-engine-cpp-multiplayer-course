#include "CWorldHealthBar.h"

#include "Components/CAttributeComponent.h"

void UCWorldHealthBar::NativeConstruct()
{
	Super::NativeConstruct();

	OnChangeHealthBar(InitialHealth / AttributeComponent->GetHealthMax());
	AttributeComponent->OnHealthChanged.AddDynamic(this, &UCWorldHealthBar::OnHealthChanged);
	AttributeComponent->OnDeath.AddDynamic(this, &UCWorldHealthBar::OnDeath);
}

void UCWorldHealthBar::OnHealthChanged(AActor* InstigatorActor, UCAttributeComponent* OwnerComponent, float NewHealth, float Delta)
{
	if (Delta == 0) return;
	if (!OwnerComponent->IsAlive()) return;

	OnChangeHealthBar(NewHealth / OwnerComponent->GetHealthMax());
}

void UCWorldHealthBar::OnChangeHealthBar_Implementation(const float HealthScalarValue)
{
	this->SetVisibility(HealthScalarValue == 1 ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);

	// Additional implementation must be handled by the WidgetBlueprint that is inheriting from this class.
}

void UCWorldHealthBar::OnDeath(AActor* KillerActor, UCAttributeComponent* OwnerComponent)
{
	OnAnimateOwnerDeath();
}

void UCWorldHealthBar::OnAnimateOwnerDeath_Implementation()
{
	// Implementation must be handled by the WidgetBlueprint that is inheriting from this class.
}
