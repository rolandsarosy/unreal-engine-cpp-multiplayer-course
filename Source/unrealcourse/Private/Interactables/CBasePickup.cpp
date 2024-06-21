#include "Interactables/CBasePickup.h"

ACBasePickup::ACBasePickup()
{
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	RootComponent = StaticMeshComponent;
	
	CooldownDuration = 10.0f;
	IsOnCooldown = false;

	bReplicates = true;
}

void ACBasePickup::Interact_Implementation(APawn* InstigatorPawn)
{
	OnAttemptPickup(InstigatorPawn);
}

void ACBasePickup::OnAttemptPickup(APawn* InstigatorPawn)
{
	if (!IsOnCooldown)
	{
		if (OnEffectTrigger(InstigatorPawn)) OnStartCooldown();
	}
}

void ACBasePickup::OnStartCooldown()
{
	IsOnCooldown = true;
	FTimerHandle TimerHandle = FTimerHandle();
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ACBasePickup::OnResetCooldown, CooldownDuration);
	// TODO - When we know more about the future uses of this base class, we can extract this behaviour to be required for overriding in children. 
	RootComponent->ToggleVisibility(true);
	SetActorEnableCollision(false);
}

void ACBasePickup::OnResetCooldown()
{
	IsOnCooldown = false;
	// TODO - When we know more about the future uses of this base class, we can extract this behaviour to be required for overriding in children. 
	RootComponent->ToggleVisibility(true);
	SetActorEnableCollision(true);
}
