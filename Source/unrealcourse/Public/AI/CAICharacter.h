#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CAICharacter.generated.h"

class UCActionComponent;
class UCWorldUserWidget;
class UCAttributeComponent;
class UPawnSensingComponent;

UCLASS(Abstract)
class UNREALCOURSE_API ACAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ACAICharacter();

	UPROPERTY(EditDefaultsOnly, Category="Coins")
	int CoinRewardUponDeath;

protected:
	UPROPERTY(VisibleAnywhere, Category="Components")
	TObjectPtr<UPawnSensingComponent> PawnSensingComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<UCAttributeComponent> AttributeComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<UCActionComponent> ActionComponent;

	UPROPERTY(EditDefaultsOnly, Category="UI")
	TSubclassOf<UUserWidget> HealthBarWidgetClass;

	virtual void PostInitializeComponents() override;

private:
	TObjectPtr<UCWorldUserWidget> ActiveHealthBar;

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UCAttributeComponent* UAttributeComponent, float NewHealth, float Delta);

	UFUNCTION()
	void OnDeath(AActor* KillerActor, UCAttributeComponent* OwnerComponent);

	UFUNCTION()
	void SetTargetActor(AActor* NewTarget, bool ShouldOverrideCurrentTarget) const;

	UFUNCTION()
	void OnSeePawn(APawn* Pawn);

	void AddHealthBar();
};
