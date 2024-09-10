#include "Interactables/CItemChest.h"
#include "Net/UnrealNetwork.h"

#define LOCTEXT_NAMESPACE "InteractableActors"

ACItemChest::ACItemChest()
{
	bReplicates = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	RootComponent = BaseMesh;

	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>("LidMesh");
	LidMesh->SetupAttachment(BaseMesh);

	bIsLocked = false;
	bIsLidOpened = false;
}

void ACItemChest::OnActorLoaded_Implementation()
{
	ICSaveableInterface::OnActorLoaded_Implementation();
	OnRep_IsLidOpened();
}

void ACItemChest::MulticastOnUnSuccessfulOpeningAttempt_Implementation()
{
	OnUnsuccessfullOpeningAttempt();
}

void ACItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	if (bIsLocked)
	{
		if (AttemptUnlock(InstigatorPawn))
		{
			bIsLocked = false;
			bIsLidOpened = true;
			OnRep_IsLidOpened();
		}
		else
		{
			MulticastOnUnSuccessfulOpeningAttempt();
		}
	}
	else
	{
		bIsLidOpened = !bIsLidOpened;
		OnRep_IsLidOpened();
	}
}

FText ACItemChest::GetInteractText_Implementation(APawn* InstigatorPawn)
{
	FText InteractTextOpen = LOCTEXT("ItemChest_Open", "Opens the chest.");
	FText InteractTextClose = LOCTEXT("ItemChest_Close", "Closes the chest.");
	FText InteractTextLocked = LOCTEXT("ItemChest_Locked", "Locked. Requires a key to open.");

	if (bIsLocked) return InteractTextLocked;

	return bIsLidOpened ? InteractTextClose : InteractTextOpen;
}

void ACItemChest::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACItemChest, bIsLidOpened)
	DOREPLIFETIME(ACItemChest, bIsLocked)
}

#undef LOCTEXT_NAMESPACE
