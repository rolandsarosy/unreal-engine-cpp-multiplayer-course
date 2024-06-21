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
 * @param Delta: The change in health
 *
 * Subscribed listeners can use these parameters to perform appropriate logic, such as displaying damage numbers or updating health bar UI.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged, AActor*, InstigatorActor, UCAttributeComponent*, OwnerComponent, float, NewHealth, float, Delta);

/** 
 * @brief Parameterized Multicast Delegate that responds to Rage changes.
 *
 * The FOnRageChanged Multicast Delegate is designed to respond to Rage changes
 * in an entity. It broadcasts to all subscribed listeners when the Rage value changes.
 * 
 * @param InstigatorActor: The Actor that initiated the Rage change, typically the "attacker". In cases of spending rage, it can be the owner itself.
 * @param OwnerComponent: The Attribute Component instance that owns the Rage attribute.
 * @param NewRage: The new Rage value after the change occurred.
 * @param Delta: The change in Rage. 
 *
 * Subscribed listeners can use these parameters to perform appropriate logic, such as displaying damage numbers or updating health bar UI.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnRageChanged, AActor*, InstigatorActor, UCAttributeComponent*, OwnerComponent, float, NewRage, float, Delta);

/** 
 * @brief Parameterized Multicast Delegate that responds to the event when health hits zero, death.
 *
 * The FOnDeath Multicast Delegate is designed to respond to when health reaches zero in an entity.
 *It broadcasts to all subscribed listeners when this happens, after the FOnHealthChanged Multicast Delegate has broadcasted. They're not exclusive.
 * 
 * @param KillerActor: The Actor killed the owner of the AttributeComponent, typically the "attacker".
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
	FOnRageChanged OnRageChanged;

	UPROPERTY(BlueprintAssignable, Category="Attributes")
	FOnDeath OnDeath;

	UFUNCTION(BlueprintCallable, Category="Attributes", meta=(Tooltip = "Applies health change to the owner."))
	bool ApplyHealthChange(AActor* InstigatorActor, float Delta);

	UFUNCTION(BlueprintCallable, Category="Attributes", meta=(Tooltip = "Applies Rage change to the owner separately from the default Rage gathering mechanics."))
	bool ApplyRageChange(AActor* InstigatorActor, float Delta);

	UFUNCTION(BlueprintCallable, Category="Attributes", meta=(Tooltip = "Returns whether the owner is currently considered alive or not."))
	bool IsAlive() const;

	UFUNCTION(BlueprintCallable, Category="Attributes", meta=(Tooltip = "Kills the owner by dealing fatal damage to them. The owners must handle their death separately. "))
	bool KillOwner(AActor* InstigatorActor);

	UFUNCTION(BlueprintCallable, Category="Attributes", meta=(DisplayName = "Get Current Health", Tooltip = "Returns the current health of the owner."))
	float GetHealthCurrent() const;

	UFUNCTION(BlueprintCallable, Category="Attributes", meta=(Displayname = "Get Max Health", Tooltip = "Returns the maximum health of the owner."))
	float GetHealthMax() const;

	UFUNCTION(BlueprintCallable, Category="Attributes", meta=(DisplayName = "Get Current Rage", Tooltip = "Returns the current Rage of the owner."))
	float GetRageCurrent() const;

	UFUNCTION(BlueprintCallable, Category="Attributes", meta=(Displayname = "Get Max Rage", Tooltip = "Returns the maximum Rage of the owner."))
	float GetRageMax() const;

private:
	UPROPERTY(EditDefaultsOnly, Replicated, Category="Attributes", meta=(Tooltip = "Signals the current health of the owner. Also marks the starting health."))
	float HealthCurrent;

	UPROPERTY(EditDefaultsOnly, Replicated, Category="Attributes", meta=(Tooltip = "Signals the max health of the owner."))
	float HealthMax;

	UPROPERTY(EditDefaultsOnly, Replicated, Category="Attributes", meta=(Tooltip = "Signals whether the owner collects and holds Rage or not."))
	bool IsRageEnabled;

	UPROPERTY(EditDefaultsOnly, Replicated, Category="Attributes", meta=(EditCondition = "IsRageEnabled == true", Tooltip = "Signals which percentage of the incoming damage will be converted to Rage."))
	int32 RageGainPercentage;

	UPROPERTY(EditDefaultsOnly, Replicated, Category="Attributes", meta=(EditCondition = "IsRageEnabled == true", Tooltip = "Signals the current amount of Rage held by the owner. Also marks the starting Rage amount."))
	float RageCurrent;

	UPROPERTY(EditDefaultsOnly, Replicated, Category="Attributes", meta=(EditCondition = "IsRageEnabled == true", Tooltip = "Signals the maximum amount of Rage the owner can ever hold."))
	float RageMax;

	UFUNCTION(NetMulticast, Reliable) // @FIXME: Mark as unreliable once the state has been moved out of the ACCharacter
	void MutlicastOnHealthChanged(AActor* InstigatorActor, float NewHealth, float Delta);

	UFUNCTION(NetMulticast, Reliable) // @FIXME: Mark as unreliable once the state has been moved out of the ACCharacter
	void MulticastOnRageChanged(AActor* InstigatorActor, float NewRage, float Delta);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastOnDeath(AActor* KillerActor);
};
