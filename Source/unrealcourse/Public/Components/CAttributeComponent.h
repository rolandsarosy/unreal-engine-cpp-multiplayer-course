#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CAttributeComponent.generated.h"

/** 
 * @brief Parameterized Multicast Delegate that responds to health changes.
 *
 * The FOnHealthChanged Multicast Delegate is designed to respond to health changes
 * in an entity. It broadcasts to all subscribed listeners when the health value changes.
 * 
 * @param InstigatorActor: The Actor that initiated the health change, typically the "attacker".
 * @param OwnerComponent: The Attribute Component instance that owns the health attribute.
 * @param NewHealth: The new health value after the change occurred.
 * @param Delta: The change in health, calculated as NewHealth minus the old health value.
 *
 * Subscribed listeners can use these parameters to perform appropriate logic, such as
 * displaying damage numbers or updating health bar UI.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged, AActor*, InstigatorActor, UCAttributeComponent*, OwnerComponent, float, NewHealth, float, Delta);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UNREALCOURSE_API UCAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCAttributeComponent();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attributes")
	float HealthCurrent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attributes")
	float HealthMax;

public:
	UPROPERTY(BlueprintAssignable, Category="Attributes")
	FOnHealthChanged OnHealthChanged;
	
	UFUNCTION(BlueprintCallable, Category="Attributes")
	bool ApplyHealthChange(float Delta);
	
	UFUNCTION(BlueprintCallable, Category="Attributes")
	bool IsAlive() const;
};
