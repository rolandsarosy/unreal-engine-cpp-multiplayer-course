#pragma once

#include "CoreMinimal.h"
#include "CoreUObject.h"
#include "InputAction.h"
#include "GameFramework/Character.h"
#include "CCharacter.generated.h"

struct FInputActionInstance;
struct FInputActionValue;
class UInputMappingContext;
class UInputAction;
class UCameraComponent;
class USpringArmComponent;

UCLASS()
class UNREALCOURSE_API ACCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ACCharacter();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputMappingContext> DefaultInputMapping;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> Input_Move;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> Input_Look;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> Input_ToggleDebug;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> Input_PrimaryAttack;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	UPROPERTY(EditAnywhere, Category="Abilities")
	TSubclassOf<AActor> PrimaryProjectile;

	bool IsDebugEnabled;

	virtual void BeginPlay() override;

	void Move(const FInputActionInstance& InputActionInstance);

	void Look(const FInputActionValue& InputActionValue);

	void ToggleIsDebugEnabled();

	void VisualizeRotations() const;

	void PrimaryAttack();
};
