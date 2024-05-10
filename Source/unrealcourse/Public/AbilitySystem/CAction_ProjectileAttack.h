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
	UPROPERTY(EditDefaultsOnly, Category="Abilities")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category="Abilities")
	TObjectPtr<UAnimMontage> AttackAnimation;

	UPROPERTY(EditDefaultsOnly, Category="Abilities")
	TObjectPtr<UParticleSystem> MuzzleFlashParticleSystem;

	UPROPERTY(VisibleAnywhere, Category="Abilities")
	FName AttackSocketName;

	UPROPERTY(VisibleAnywhere, Category="Abilities")
	FName AttackAnimationMontageNotifyStart;

	UPROPERTY(VisibleAnywhere, Category="Abilities")
	FName AttackAnimationMontageNotifyEnd;

	UFUNCTION()
	void SpawnProjectile(ACharacter* InstigatorCharacter) const;

private:
	UFUNCTION()
	void OnMontageNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);

	FRotator TraceForProjectileSpawnRotator(ACharacter* InstigatorCharacter) const;
};
