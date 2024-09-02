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
	ICGameplayInterface::OnActorLoaded_Implementation();

	OnRep_IsLidOpened();
}

bool ACItemChest::AttemptUnlock_Implementation(AActor* InstigatorActor)
{
	// This implementation will be done by children of this class. This function mustn't be marked with the PURE_VIRTUAL macro as it uses Unreal's Reflection system and has the BlueprintNative flag.
	return true;
}

void ACItemChest::MulticastOnUnSuccessfulOpeningAttempt_Implementation()
{
	OnUnsuccessfullOpeningAttempt();
}

void ACItemChest::OnUnsuccessfullOpeningAttempt_Implementation()
{
	// This implementation will be done by children of this class. This function mustn't be marked with the PURE_VIRTUAL macro as it uses Unreal's Reflection system and has the BlueprintNative flag.
}

void ACItemChest::OnRep_IsLidOpened_Implementation()
{
	// This implementation will be done by children of this class. This function mustn't be marked with the PURE_VIRTUAL macro as it uses Unreal's Reflection system and has the BlueprintNative flag.
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
}

#undef LOCTEXT_NAMESPACE
