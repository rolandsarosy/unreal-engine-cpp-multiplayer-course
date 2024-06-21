#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "InputAction.h"
#include "GameFramework/Character.h"
#include "CCharacter.generated.h"

class UCActionComponent;
struct FInputActionInstance;
struct FInputActionValue;
class UCAttributeComponent;
class UCInteractionComponent;
class UInputMappingContext;
class UInputAction;
class UCameraComponent;
class USpringArmComponent;

UCLASS(Abstract)
class UNREALCOURSE_API ACCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ACCharacter();

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

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> Input_Sprint;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> Input_Parry;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	FGameplayTag ActionTag_PrimaryAttack;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	FGameplayTag ActionTag_SpecialAttack;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	FGameplayTag ActionTag_TeleportAttack;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	FGameplayTag ActionTag_Sprint;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	FGameplayTag ActionTag_Parry;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	UPROPERTY(VisibleAnywhere, Category="Components")
	TObjectPtr<UCInteractionComponent> InteractionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<UCActionComponent> ActionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<UCAttributeComponent> AttributeComponent;
private:
	FTimerHandle TimerHandle_Attack;

	UFUNCTION()
	void OnHealthChanged(AActor* Actor, UCAttributeComponent* UAttributeComponent, float NewHealth, float Delta);

	UFUNCTION()
	void OnDeath(AActor* KillerActor, UCAttributeComponent* OwnerComponent);

	virtual FVector GetPawnViewLocation() const override;

	void Move(const FInputActionInstance& InputActionInstance);

	void Look(const FInputActionValue& InputActionValue);

	void SprintStart();

	void SprintStop();

	void PrimaryAttack();

	void SpecialAttack();

	void TeleportAttack();

	void Parry();

	virtual void PostInitializeComponents() override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
};
