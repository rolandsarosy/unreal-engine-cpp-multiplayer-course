#include "Interactables/CBasePickup.h"

#include "Net/UnrealNetwork.h"

ACBasePickup::ACBasePickup()
{
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	RootComponent = StaticMeshComponent;
	bReplicates = true;

	CooldownDuration = 10.0f;
	IsOnCooldown = false;
}

void ACBasePickup::Interact_Implementation(APawn* InstigatorPawn)
{
	OnAttemptPickup(InstigatorPawn);
}

void ACBasePickup::OnAttemptPickup(APawn* InstigatorPawn)
{
	if (!IsOnCooldown && OnEffectTrigger(InstigatorPawn)) { OnStartCooldown(); }
}

void ACBasePickup::OnRep_IsOnCooldown()
{
	if (IsOnCooldown)
	{
		RootComponent->ToggleVisibility(true);
		SetActorEnableCollision(false);
	}
	else
	{
		RootComponent->ToggleVisibility(true);
		SetActorEnableCollision(true);
	}
}

/**  Can only run on the server. */
void ACBasePickup::OnStartCooldown()
{
	if (!ensure(GetOwner()->HasAuthority())) return;

	IsOnCooldown = true;
	OnRep_IsOnCooldown(); // OnRep_Foo() functions don't trigger automatically on the server. Clients will ignore the double call of the function as they already have the correct state set.

	FTimerHandle TimerHandle = FTimerHandle();
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ACBasePickup::OnResetCooldown, CooldownDuration);
}

/**  Can only run on the server. */
void ACBasePickup::OnResetCooldown()
{
	if (!ensure(GetOwner()->HasAuthority())) return;

	IsOnCooldown = false;
	OnRep_IsOnCooldown(); // OnRep_Foo() functions don't trigger automatically on the server. Clients will ignore the double call of the function as they already have the correct state set.
}

void ACBasePickup::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACBasePickup, IsOnCooldown);
}
