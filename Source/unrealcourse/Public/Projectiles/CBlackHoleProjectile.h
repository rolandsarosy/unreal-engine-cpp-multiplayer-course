#pragma once

#include "CoreMinimal.h"
#include "Projectiles/CBaseProjectile.h"
#include "CBlackHoleProjectile.generated.h"

class URadialForceComponent;

UCLASS(Abstract)
class UNREALCOURSE_API ACBlackHoleProjectile : public ACBaseProjectile
{
	GENERATED_BODY()

public:
	ACBlackHoleProjectile();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<URadialForceComponent> RadialForceComponent;
};
