#pragma once

#include "CoreMinimal.h"
#include "Interactables/CBasePickup.h"
#include "CAbilityPickup.generated.h"

class UCBaseAction;
class UCActionAbility;

UCLASS()
class UNREALCOURSE_API ACAbilityPickup : public ACBasePickup
{
	GENERATED_BODY()

protected:
	virtual bool OnEffectTrigger(APawn* InstigatorPawn) override;

private:
	UPROPERTY(EditDefaultsOnly, Category="Pickup")
	TSubclassOf<UCBaseAction> GrantsAbility;
};
