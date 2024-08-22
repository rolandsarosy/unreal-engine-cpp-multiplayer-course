#include "CAnimInstance.h"

#include "Components/CActionComponent.h"
#include "Components/CAttributeComponent.h"

void UCAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	AActor* OwningActor = GetOwningActor();
	if (!OwningActor) return;

	ActionComponent = UCActionComponent::GetComponentFrom(OwningActor);
	AttributeComponent = UCAttributeComponent::GetComponentFrom(OwningActor);
}
