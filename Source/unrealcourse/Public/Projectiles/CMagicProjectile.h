#pragma once

#include "CoreMinimal.h"
#include "Projectiles/CBaseProjectile.h"
#include "CMagicProjectile.generated.h"

class USoundCue;
class UAudioComponent;
class UParticleSystem;

UCLASS()
class UNREALCOURSE_API ACMagicProjectile : public ACBaseProjectile
{
	GENERATED_BODY()

public:
	ACMagicProjectile();
	
	virtual void PostInitializeComponents() override;

	virtual void BeginPlay() override;

protected:
	UFUNCTION()
	void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(EditDefaultsOnly, Category="Audio")
	TObjectPtr<UAudioComponent> LoopingAudioComponent;
	
	UPROPERTY(EditDefaultsOnly, Category="Audio")
	TObjectPtr<USoundCue> ImpactSoundCue;
	
	UPROPERTY(EditDefaultsOnly, Category="Effects")
	TObjectPtr<UParticleSystem> ImpactParticleSystem; 
	
	UPROPERTY(EditDefaultsOnly, Category="Damage")
	float DamageAmount;
};
