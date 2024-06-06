#pragma once

#include "CoreMinimal.h"
#include "Components/CAttributeComponent.h"
#include "GAS/CActionEffect.h"
#include "CActionEffect_Thorns.generated.h"

UCLASS()
class UNREALCOURSE_API UCActionEffect_Thorns : public UCActionEffect
{
	GENERATED_BODY()

public:
	UCActionEffect_Thorns();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Effect")
	int32 ThornsPercentage;

	virtual void StartAction_Implementation(AActor* Instigator) override;

	virtual void StopAction_Implementation(AActor* Instigator) override;

private:
	UFUNCTION()
	void OnOwnerHealthChanged(AActor* InstigatorActor, UCAttributeComponent* OwnerComponent, float NewHealth, float Delta);
};
