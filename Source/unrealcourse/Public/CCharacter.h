#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "GameFramework/Character.h"
#include "CCharacter.generated.h"

struct FInputActionInstance;
struct FInputActionValue;
class UCAttributeComponent;
class UCInteractionComponent;
class UInputMappingContext;
class UInputAction;
class UCameraComponent;
class USpringArmComponent;
class UAnimMontage;

UCLASS(Abstract)
class UNREALCOURSE_API ACCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ACCharacter();

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	UFUNCTION(Exec) // This is a cheat that'll work only in non-shipped builds. Conditional compiling is unnecessary as the console is disabled in shipped builds.
	void HealSelf(float Amount = 100.0f);

protected:
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputMappingContext> DefaultInputMapping;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> Input_Move;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> Input_Look;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> Input_PrimaryAttack;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> Input_PrimaryInteract;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> Input_SpecialAttack;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> Input_TeleportAttack;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> Input_Jump;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	UPROPERTY(VisibleAnywhere, Category="Components")
	TObjectPtr<UCInteractionComponent> InteractionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<UCAttributeComponent> AttributeComponent;

	UPROPERTY(EditAnywhere, Category="Abilities")
	TSubclassOf<AActor> PrimaryAttackProjectile;

	UPROPERTY(EditAnywhere, Category="Abilities")
	TSubclassOf<AActor> SpecialAttackProjectile;

	UPROPERTY(EditAnywhere, Category="Abilities")
	TSubclassOf<AActor> TeleportAttackProjectile;

	UPROPERTY(EditAnywhere, Category="Abilities")
	TObjectPtr<UAnimMontage> AttackAnimation;

	UPROPERTY(EditDefaultsOnly, Category="Abilities")
	TObjectPtr<UParticleSystem> MuzzleFlashParticleSystem;

	UPROPERTY(EditDefaultsOnly, Category="Abilities")
	FName AttackSocketName;

private:
	FTimerHandle TimerHandle_Attack;

	UFUNCTION()
	void OnHealthChanged(AActor* Actor, UCAttributeComponent* UAttributeComponent, float NewHealth, float Delta);

	UFUNCTION()
	void OnDeath(AActor* KillerActor, UCAttributeComponent* OwnerComponent);

	virtual FVector GetPawnViewLocation() const override;
	
	FRotator TraceForProjectileSpawnRotator() const;
	
	void Move(const FInputActionInstance& InputActionInstance);

	void Look(const FInputActionValue& InputActionValue);

	void PrimaryAttack_Start();

	void PrimaryAttack_TimeElapsed();

	void SpecialAttack_Start();

	void SpecialAttack_TimeElapsed();

	void TeleportAttack_Start();

	void TeleportAttack_TimeElapsed();

	virtual void PostInitializeComponents() override;
};
