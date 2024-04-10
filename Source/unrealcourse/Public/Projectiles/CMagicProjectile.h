#pragma once

#include "CoreMinimal.h"
#include "Projectiles/CBaseProjectile.h"
#include "CMagicProjectile.generated.h"

UCLASS()
class UNREALCOURSE_API ACMagicProjectile : public ACBaseProjectile
{
	GENERATED_BODY()

public:
	ACMagicProjectile();

	virtual void PostInitializeComponents() override;

protected:
	UFUNCTION()
	void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	float DamageAmount;
};
