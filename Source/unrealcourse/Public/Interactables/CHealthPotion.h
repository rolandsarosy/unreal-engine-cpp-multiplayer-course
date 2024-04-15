#pragma once

#include "CoreMinimal.h"
#include "Interactables/CBasePickup.h"
#include "CHealthPotion.generated.h"

UCLASS()
class UNREALCOURSE_API ACHealthPotion : public ACBasePickup
{
	GENERATED_BODY()

public:
	ACHealthPotion();

	UPROPERTY(EditDefaultsOnly, Category="Pickup")
	float HealthRestoreAmount;
	
protected:
	virtual bool OnEffectTrigger(APawn* InstigatorPawn) override;
};
