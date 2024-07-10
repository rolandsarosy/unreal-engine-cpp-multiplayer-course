#pragma once

#include "CoreMinimal.h"
#include "CWorldUserWidget.h"
#include "CWorldHealthBar.generated.h"

class UCAttributeComponent;

UCLASS(Abstract)
class UNREALCOURSE_API UCWorldHealthBar : public UCWorldUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category="UI", meta=(ExposeOnSpawn = true))
	float InitialHealth;

	UPROPERTY(BlueprintReadOnly, Category="UI", meta=(ExposeOnSpawn = true))
	UCAttributeComponent* AttributeComponent;

protected:
	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UCAttributeComponent* OwnerComponent, float NewHealth, float Delta);

	UFUNCTION()
	void OnDeath(AActor* KillerActor, UCAttributeComponent* OwnerComponent);

	/**
	 * @brief Called when the health bar needs to be updated.
	 *
	 * This event is triggered when the health of the owner changes. The health value is converted to a scalar value between 0 and 1.
	 * The updated health scalar value is passed as a parameter to this event.
	 *
	 * @param HealthScalarValue The new health scalar value between 0 and 1.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnChangeHealthBar(float HealthScalarValue);

	/**
	 * @brief Called when the HealthBar's owner has died
	 *
	 * The event is called after the Owner's AttributeComponent has triggered OnDeath. It signals that the implementing blueprint shoul
	 * animate and remove the HealthBar from the View tree.
	 *
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnAnimateOwnerDeath();
	
	virtual void NativeConstruct() override;
};
