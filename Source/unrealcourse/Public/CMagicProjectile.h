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

	virtual void Tick(float DeltaTime) override;
	
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> SphereComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UParticleSystemComponent> ParticleSystemComponent;

	virtual void BeginPlay() override;
};
