#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CAICharacter.generated.h"

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

	virtual void PostInitializeComponents() override;

private:
	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UCAttributeComponent* UAttributeComponent, float NewHealth, float Delta);

	void SetTargetActor(AActor* NewTarget) const;
};
