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
 * @param InstigatorActor: The Actor that initiated the health change, typically the "attacker". In cases of healing, it can be itself or the healing item or Actor who heals.
 * @param OwnerComponent: The Attribute Component instance that owns the health attribute.
 * @param NewHealth: The new health value after the change occurred.
 * @param Delta: The change in health, calculated as NewHealth minus the old health value.
 *
 * Subscribed listeners can use these parameters to perform appropriate logic, such as
 * displaying damage numbers or updating health bar UI.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged, AActor*, InstigatorActor, UCAttributeComponent*, OwnerComponent, float, NewHealth, float, Delta);

/** 
 * @brief Parameterized Multicast Delegate that responds to the event when health hits zero, death.
 *
 * The FOnDeath Multicast Delegate is designed to respond to when health reaches zero
 * in an entity. It broadcasts to all subscribed listeners when this happens, after the FOnHealthChanged Multicast Delegate has broadcasted. They're not exclusive.
 * 
 * @param KillerActor: The Actor that initiated the health change, typically the "attacker". In cases of healing, it can be itself or the healing item or Actor who heals.
 * @param OwnerComponent: The Attribute Component instance that owns the health attribute.
 *
 * Subscribed listeners can use these parameters to perform appropriate logic, such as disabling input, setting ragdolling or a new lifespan.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDeath, AActor*, KillerActor, UCAttributeComponent*, OwnerComponent);

UCLASS(ClassGroup=(Custom))
class UNREALCOURSE_API UCAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCAttributeComponent();

	UFUNCTION(BlueprintCallable, Category="Attributes", meta=(DisplayName = "Get AttributeComponent From Actor", Tooltip = "Returns AttributeComponent if the Actor has any. Otherwise a nullptr."))
	static UCAttributeComponent* GetComponentFrom(AActor* FromActor);

	UPROPERTY(BlueprintAssignable, Category="Attributes")
	FOnHealthChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category="Attributes")
	FOnDeath OnDeath;

	UFUNCTION(BlueprintCallable, Category="Attributes")
	bool ApplyHealthChange(AActor* InstigatorActor, float Delta);

	UFUNCTION(BlueprintCallable, Category="Attributes")
	bool IsAlive() const;

	UFUNCTION(BlueprintCallable, Category="Attributes")
	bool KillOwner(AActor* InstigatorActor);

	UFUNCTION(BlueprintCallable, Category="Attributes", meta= (DisplayName = "Get Current Health"))
	float GetHealthCurrent() const;

	UFUNCTION(BlueprintCallable, Category="Attributes", meta =(Displayname = "Get Max Health"))
	float GetHealthMax() const;

private:
	UPROPERTY(EditDefaultsOnly, Category="Attributes")
	float HealthCurrent;

	UPROPERTY(EditDefaultsOnly, Category="Attributes")
	float HealthMax;
};
