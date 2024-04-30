#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CAICharacter.generated.h"

class UCWorldUserWidget;
class UCAttributeComponent;
class UPawnSensingComponent;

UCLASS()
class UNREALCOURSE_API ACAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ACAICharacter();

protected:
	UPROPERTY(VisibleAnywhere, Category="Components")
	TObjectPtr<UPawnSensingComponent> PawnSensingComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<UCAttributeComponent> AttributeComponent;

	UPROPERTY(EditDefaultsOnly, Category="UI")
	TSubclassOf<UUserWidget> HealthBarWidgetClass;
	
	virtual void PostInitializeComponents() override;

private:
	TObjectPtr<UCWorldUserWidget> ActiveHealthBar;

	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UCAttributeComponent* UAttributeComponent, float NewHealth, float Delta);

	UFUNCTION()
	void OnDeath(AActor* KillerActor, UCAttributeComponent* OwnerComponent);
	
	void SetTargetActor(AActor* NewTarget) const;

	void AddHealthBar();
};
