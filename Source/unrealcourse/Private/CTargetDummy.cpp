#include "CTargetDummy.h"
#include "Components/CAttributeComponent.h"

ACTargetDummy::ACTargetDummy()
{
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	RootComponent = StaticMeshComponent;

	AttributeComponent = CreateDefaultSubobject<UCAttributeComponent>("AttributeComponent");
}

void ACTargetDummy::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	AttributeComponent->OnHealthChanged.AddDynamic(this, &ACTargetDummy::OnHealthChanged);
}

void ACTargetDummy::OnHealthChanged(AActor* Actor, UCAttributeComponent* UAttributeComponent, float NewHealth, float Delta)
{
	if (Delta < 0.0f)
	{
		StaticMeshComponent->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);
	}
}

