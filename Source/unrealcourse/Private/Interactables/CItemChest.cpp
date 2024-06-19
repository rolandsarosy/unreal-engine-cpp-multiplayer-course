#include "Interactables/CItemChest.h"

#include "Net/UnrealNetwork.h"

ACItemChest::ACItemChest()
{
	bReplicates = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	RootComponent = BaseMesh;

	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>("LidMesh");
	LidMesh->SetupAttachment(BaseMesh);

	IsLidOpened = false;
}

void ACItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	IsLidOpened = !IsLidOpened;
}

void ACItemChest::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACItemChest, IsLidOpened)
}
