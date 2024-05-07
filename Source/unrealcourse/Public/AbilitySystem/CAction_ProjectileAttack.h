#pragma once

#include "CoreMinimal.h"
#include "CAction.h"
#include "CAction_ProjectileAttack.generated.h"

UCLASS(Abstract)
class UNREALCOURSE_API UCAction_ProjectileAttack : public UCAction
{
	GENERATED_BODY()

public:
	UCAction_ProjectileAttack();

	virtual void StartAction_Implementation(AActor* Instigator) override;

protected:
	UPROPERTY(EditAnywhere, Category="Abilities")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditAnywhere, Category="Abilities")
	TObjectPtr<UAnimMontage> AttackAnimation;

	UPROPERTY(EditDefaultsOnly, Category="Abilities")
	TObjectPtr<UParticleSystem> MuzzleFlashParticleSystem;

	UPROPERTY(VisibleDefaultsOnly, Category="Abilities")
	FName AttackSocketName;

	UFUNCTION()
	void AttackDelay_TimeElapsed(ACharacter* InstigatorCharacter);

private:
	FRotator TraceForProjectileSpawnRotator(ACharacter* InstigatorCharacter) const;
};
