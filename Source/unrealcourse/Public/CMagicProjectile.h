#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CMagicProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UParticleSystemComponent;

UCLASS()
class UNREALCOURSE_API ACMagicProjectile : public AActor
{
	GENERATED_BODY()

public:
	ACMagicProjectile();
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USphereComponent> SphereComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UParticleSystemComponent> ParticleSystemComponent;
};
