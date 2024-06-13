#include "Interactables/CItemChest.h"

#include "Net/UnrealNetwork.h"

ACItemChest::ACItemChest()
{
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	RootComponent = BaseMesh;

	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>("LidMesh");
	LidMesh->SetupAttachment(BaseMesh);

	bReplicates = true;
}

void ACItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	IsLidOpened = !IsLidOpened;
	OnRep_IsLidOpened();
}

void ACItemChest::OnRep_IsLidOpened() const
{
	LidMesh->SetRelativeRotation(FRotator(IsLidOpened ? 110 : 0, 0, 0));
}

void ACItemChest::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACItemChest, IsLidOpened)
}
