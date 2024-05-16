#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Projectiles/CBaseProjectile.h"
#include "CMagicProjectile.generated.h"

class UCBaseAction;
class UCActionEffect;
class USoundCue;
class UAudioComponent;
class UParticleSystem;

UCLASS(Abstract)
class UNREALCOURSE_API ACMagicProjectile : public ACBaseProjectile
{
	GENERATED_BODY()

public:
	ACMagicProjectile();

	virtual void PostInitializeComponents() override;

private:
	UPROPERTY(EditDefaultsOnly, Category="Audio")
	TObjectPtr<UAudioComponent> LoopingAudioComponent;

	UPROPERTY(EditDefaultsOnly, Category="Audio")
	TObjectPtr<USoundCue> ImpactSoundCue;

	UPROPERTY(EditDefaultsOnly, Category="Effects")
	TObjectPtr<UParticleSystem> ImpactParticleSystem;

	UPROPERTY(EditDefaultsOnly, Category="Effects")
	TSubclassOf<UCameraShakeBase> ImpactCameraShake;

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	float DamageAmount;

	UPROPERTY(EditDefaultsOnly, Category="Damage", meta=(Tooltip = "Describes how many times (exclusive) the projectile can be bounced back-and-forth before parrying is ignored."))
	uint8 MaxParryAmount;

	UPROPERTY(EditDefaultsOnly, Category="Damage", meta=(Tooltip = "Should be set to the ability system's Parry tag. If this tag is present, the projectile will be reflected."))
	FGameplayTag ParryTag;

	UPROPERTY(EditDefaultsOnly, Category="Damage", meta=(Tooltip = "Applies the given effect on sucessfully damaging an Actor who is holding an ActionComponent."))
	TSubclassOf<UCActionEffect> AppliedEffectClass;

	uint8 CurrentParryAmount;

	UFUNCTION()
	void OnComponentOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void PlayEffectsAndDestroy();
};
