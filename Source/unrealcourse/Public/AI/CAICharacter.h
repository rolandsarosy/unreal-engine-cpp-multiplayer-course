#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CAICharacter.generated.h"

class UCWorldHealthBar;
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
	TSubclassOf<UCWorldHealthBar> HealthBarWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category="UI")
	TSubclassOf<UUserWidget> SpottedPopupWidgetClass;

	virtual void PostInitializeComponents() override;

private:
	TObjectPtr<UCWorldHealthBar> ActiveHealthBar;

	bool bHasSeenPlayers;

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UCAttributeComponent* UAttributeComponent, float NewHealth, float Delta);

	UFUNCTION()
	void OnDeath(AActor* KillerActor, UCAttributeComponent* OwnerComponent);

	UFUNCTION()
	void SetTargetActor(AActor* NewTarget, bool bShouldOverrideCurrentTarget) const;

	UFUNCTION()
	void OnSeePawn(APawn* Pawn);

	UFUNCTION(Unreliable, NetMulticast)
	void MulticastAddHealthBar(const float InitialHealth, UCAttributeComponent* OwnerAttributeComponent);
	
	UFUNCTION(Unreliable, NetMulticast)
	void MutlicastAddSpottedWidgetConditionally(const AActor* InstigatorActor);
};
