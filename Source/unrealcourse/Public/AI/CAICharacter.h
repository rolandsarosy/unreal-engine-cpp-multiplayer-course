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

	UFUNCTION()
	void SetCoinRewardUponDeath(int NewCoinRewardUponDeath);

	UFUNCTION()
	int GetCoinRewardUponDeath() const;

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

private:
	TObjectPtr<UCWorldHealthBar> ActiveHealthBar;

	bool bHasSeenPlayers;
	
	int CoinRewardUponDeath;

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

	virtual void PostInitializeComponents() override;
};
