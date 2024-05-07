#pragma once

#include "CoreMinimal.h"
#include "CAction.generated.h"

/**
 * @class UCAction
 * @brief The base class for all GAS-like actions in the game.
 *
 * This class provides a common interface and functionality for starting and stopping actions.
 * Each action has an ActionName property that can be used to start and stop actions without a reference to the object.
 *
 * @note All Actions should be placed inside the @AActor's @CActionComponent. 
 */
UCLASS(Blueprintable, Abstract)
class UNREALCOURSE_API UCAction : public UObject
{
	GENERATED_BODY()

public:
	/** Action name to start/stop without a reference to the object. **/
	UPROPERTY(EditDefaultsOnly, Category="Action")
	FName ActionName;

	UFUNCTION(BlueprintNativeEvent, Category="Action")
	void StartAction(AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent, Category="Action")
	void StopAction(AActor* Instigator);

	virtual UWorld* GetWorld() const override;
};
