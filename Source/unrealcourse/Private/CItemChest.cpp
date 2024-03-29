#include "CItemChest.h"

ACItemChest::ACItemChest()
{
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	RootComponent = BaseMesh;

	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>("LidMesh");
	LidMesh->SetupAttachment(BaseMesh);
}

void ACItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0F, FColor::Red, TEXT("ItemChest base class interaction interface triggered."));
}
