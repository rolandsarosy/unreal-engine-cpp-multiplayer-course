#pragma once

#include "CoreMinimal.h"
#include "Projectiles/CBaseProjectile.h"
#include "CTeleportProjectile.generated.h"

class UParticleSystemComponent;

UCLASS(Abstract)
class UNREALCOURSE_API ACTeleportProjectile : public ACBaseProjectile
{
	GENERATED_BODY()

public:
	ACTeleportProjectile();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UParticleSystemComponent> TeleportExplosionEffectComponent;

private:
	// TODO: Fix this when it is confirmed that the class will not make significant changes to this anymore.
	// This is a bad approach in my opinion, but this is what the assignment specified. I'd have rather used a delegate listening to the end of one of the particle system's emitters.
	FTimerHandle TimerHandle_TeleportEffect;
	FTimerHandle TimerHandle_TeleportAction;

	UFUNCTION()
	void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	void OnStartTeleportEffect();

	void OnTeleport();

	void OnProjectileExplosion(const AActor* HitActor);
	
	virtual void PostInitializeComponents() override;

	virtual void BeginPlay() override;
};
