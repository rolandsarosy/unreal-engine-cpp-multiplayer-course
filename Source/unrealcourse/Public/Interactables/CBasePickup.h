#pragma once

#include "CoreMinimal.h"
#include "CGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "CBasePickup.generated.h"

UCLASS(Abstract)
class UNREALCOURSE_API ACBasePickup : public AActor, public ICGameplayInterface
{
	GENERATED_BODY()
	
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

protected:
	ACBasePickup();

	UPROPERTY(EditDefaultsOnly, Category="Pickup")
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

	UPROPERTY(EditDefaultsOnly, Category="Pickup")
	float CooldownDuration;

	virtual bool OnEffectTrigger(APawn* InstigatorPawn) PURE_VIRTUAL(ACBasePickup::OnEffectTrigger, return false;);

private:
	bool IsOnCooldown;

	void OnAttemptPickup(APawn* InstigatorPawn);

	void OnStartCooldown();
	
	void OnResetCooldown();
};
