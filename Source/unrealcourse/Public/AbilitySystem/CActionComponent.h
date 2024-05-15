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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Tags")
	FGameplayTagContainer ActiveGameplayTags;

	UFUNCTION(BlueprintCallable, Category="Actions")
	void AddAction(TSubclassOf<UCAction> ActionClass, AActor* Instigator);

	UFUNCTION(BlueprintCallable, Category="Actions")
	void RemoveAction(UCAction* ActionToRemove);

	UFUNCTION(BlueprintCallable, Category="Actions")
	bool StartActionByTag(AActor* Instigator, FGameplayTag Tag);

	UFUNCTION(BlueprintCallable, Category="Actions")
	bool StopActionByTag(AActor* Instigator, FGameplayTag Tag);

	UFUNCTION(BlueprintCallable, Category="Tags", meta=(Tooltip = "Should be used for adding/removing keycard-style GameplayTags."))
	void AddGameplayTag(FGameplayTag Tag);

	UFUNCTION(BlueprintCallable, Category="Tags", meta=(Tooltip = "Should be used for adding/removing keycard-style GameplayTags."))
	bool RemoveGameplayTag(FGameplayTag Tag);

private:
	UPROPERTY(VisibleAnywhere)
	TArray<TObjectPtr<UCAction>> CurrentActions;

	UPROPERTY(EditAnywhere, Category="Actions")
	TArray<TSubclassOf<UCAction>> DefaultActions;

	virtual void BeginPlay() override;
};
