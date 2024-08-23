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

	virtual FText GetInteractText_Implementation(APawn* InstigatorPawn) override;
	
public:
	ACBasePickup();

protected:
	UPROPERTY(EditDefaultsOnly, Category="Pickup")
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

	UPROPERTY(EditDefaultsOnly, Category="Pickup")
	float CooldownDuration;

	/**
	 * @brief Called when the effect of the pickup is triggered by an instigator pawn.
	 *
	 * This method is a pure virtual function that needs to be implemented in derived classes.
	 *
	 * @param InstigatorPawn The pawn that triggered the effect.
	 *
	 * @return True if the effect was successfully triggered, false otherwise.
	 */
	virtual bool OnEffectTrigger(APawn* InstigatorPawn) PURE_VIRTUAL(ACBasePickup::OnEffectTrigger, return false;);

private:
	UPROPERTY(ReplicatedUsing="OnRep_IsOnCooldown")
	bool bIsOnCooldown;

	UFUNCTION()
	void OnRep_IsOnCooldown();

	void OnAttemptPickup(APawn* InstigatorPawn);

	void OnStartCooldown();

	void OnResetCooldown();
};
