#include "Interactables/CBasePickup.h"

#include "Framework/CGameModeBase.h"
#include "Net/UnrealNetwork.h"

ACBasePickup::ACBasePickup()
{
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	RootComponent = StaticMeshComponent;

	CooldownDuration = 10.0f;
	
	bReplicates = true;
	bIsOnCooldown = false;
	bGenerateOverlapEventsDuringLevelStreaming = false;
}

void ACBasePickup::BeginPlay()
{
	Super::BeginPlay();

	// This handles cases where pickup items were pre-placed in the world before runtime and thus had no Owners for replication.
	if (GetOwner() == nullptr && HasAuthority())
	{
		SetOwner(Cast<AActor>(GetWorld()->GetAuthGameMode<ACGameModeBase>()));
	}
}

void ACBasePickup::Interact_Implementation(APawn* InstigatorPawn)
{
	OnAttemptPickup(InstigatorPawn);
}

FText ACBasePickup::GetInteractText_Implementation(APawn* InstigatorPawn)
{
	return FText::GetEmpty();
}

void ACBasePickup::OnAttemptPickup(APawn* InstigatorPawn)
{
	if (!bIsOnCooldown && OnEffectTrigger(InstigatorPawn)) { OnStartCooldown(); }
}

void ACBasePickup::OnRep_IsOnCooldown()
{
	SetActorEnableCollision(!bIsOnCooldown);
	RootComponent->SetVisibility(!bIsOnCooldown, true);
}

/**  Can only run on the server. */
void ACBasePickup::OnStartCooldown()
{
	if (!ensure(GetOwner()->HasAuthority())) return;

	bIsOnCooldown = true;
	OnRep_IsOnCooldown(); // OnRep_Foo() functions don't trigger automatically on the server. Clients will ignore the double call of the function as they already have the correct state set.

	FTimerHandle TimerHandle = FTimerHandle();
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ACBasePickup::OnResetCooldown, CooldownDuration);
}

/**  Can only run on the server. */
void ACBasePickup::OnResetCooldown()
{
	if (!ensure(GetOwner()->HasAuthority())) return;

	bIsOnCooldown = false;
	OnRep_IsOnCooldown(); // OnRep_Foo() functions don't trigger automatically on the server. Clients will ignore the double call of the function as they already have the correct state set.
}

void ACBasePickup::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACBasePickup, bIsOnCooldown);
}
