#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "CActionComponent.generated.h"

class UCAction;

/**
 * @class UCActionComponent
 * @brief The class for managing GAS-like actions of an actor.
 *
 * The @UCActionComponent class provides functionality to add, start, and stop actions for an @AActor.
 * It manages a collection of @UCAction objects and provides convenient methods for interacting with them.
 */
UCLASS(ClassGroup=(Custom))
class UNREALCOURSE_API UCActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tags")
	FGameplayTagContainer ActiveGameplayTags;

	UFUNCTION(BlueprintCallable, Category="Actions")
	void AddAction(TSubclassOf<UCAction> ActionClass);

	UFUNCTION(BlueprintCallable, Category="Actions")
	bool StartActionByName(AActor* Instigator, FName ActionName);

	UFUNCTION(BlueprintCallable, Category="Actions")
	bool StopActionByName(AActor* Instigator, FName ActionName);

private:
	UPROPERTY()
	TArray<TObjectPtr<UCAction>> CurrentActions;

	UPROPERTY(EditAnywhere, Category="Actions")
	TArray<TSubclassOf<UCAction>> DefaultActions;

	UCActionComponent();
	
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
