#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "CActionComponent.generated.h"

class UCBaseAction;

/** 
 * @brief Parameterized Multicast Delegate that responds to an @UCBaseAction being added to an @UCActionComponent.

 * @param UCBaseAction: The action being added to the component.
 * @param AActor: The instigator, who is responsible for this action being added to the component.
 *
 * Subscribed listeners can use this, in combination with the other delegates, to react to changes to the owner's abilities and effects.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnActionAdded, UCBaseAction*, ActionAdded, AActor*, Instigator);

/** 
 * @brief Parameterized Multicast Delegate that responds to a @UCBaseAction being removed from an @UCActionComponent.
 *
 * @param UCBaseAction: The action being removed from the component.
 * @param AActor: The instigator, who is responsible for this action being removed from the component.
 *
 * Subscribed listeners can use this, in combination with the other delegates, to react to changes to the owner's abilities and effects.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnActionRemoved, UCBaseAction*, ActionRemoved, AActor*, Instigator);

/** 
 * @brief Parameterized Multicast Delegate that responds to an existing @UCBaseAction being started inside an @UCActionComponent
 *
 * @note That actions such as effects that are auto started will trigger this delegate, right after being added to the @UCActionComponent.
 * 
 * @param UCBaseAction: The action being started inside component.
 * @param AActor: The instigator, who is responsible for starting this action.
 *
 * Subscribed listeners can use this, in combination with the other delegates, to react to changes to the owner's abilities and effects.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnActionStarted, UCBaseAction*, ActionStarted, AActor*, Instigator);

/** 
 * @brief Parameterized Multicast Delegate that responds to an existing @UCBaseAction being stopped inside an @UCActionComponent
 *
 * @param UCBaseAction: The action being stopped inside component.
 * @param AActor: The instigator, who is responsible for stopping this action.
 *
 * Subscribed listeners can use this, in combination with the other delegates, to react to changes to the owner's abilities and effects.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnActionStopped, UCBaseAction*, ActionStopped, AActor*, Instigator);

/** 
 * @brief Parameterized Multicast Delegate that responds a @FGameplayTag being added to the @UCActionComponent.
 *
 * @note That only keycard-style tags should be used.
 *
 * @param FGameplayTag: The @FGameplayTag being added.
 *
 * Subscribed listeners can use this, in combination with the other delegates, to react to changes to the owner's abilities and effects.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameplayTagAdded, FGameplayTag, TagAdded);

/** 
 * @brief Parameterized Multicast Delegate that responds a @FGameplayTag being removed from the @UCActionComponent.
 *
 * @note That only keycard-style tags should be used.
 *
 * @param FGameplayTag: The @FGameplayTag being removed.
 *
 * Subscribed listeners can use this, in combination with the other delegates, to react to changes to the owner's abilities and effects.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameplayTagRemoved, FGameplayTag, TagRemoved);

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

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category="Actions")
	TArray<TObjectPtr<UCBaseAction>> CurrentActions;

	UPROPERTY(BlueprintAssignable, Category="Actions")
	FOnActionAdded OnActionAdded;

	UPROPERTY(BlueprintAssignable, Category="Actions")
	FOnActionRemoved OnActionRemoved;

	UPROPERTY(BlueprintAssignable, Category="Actions")
	FOnActionStarted OnActionStarted;

	UPROPERTY(BlueprintAssignable, Category="Actions")
	FOnActionStopped OnActionStopped;

	UPROPERTY(BlueprintAssignable, Category="Actions")
	FOnGameplayTagAdded OnGameplayTagAdded;

	UPROPERTY(BlueprintAssignable, Category="Actions")
	FOnGameplayTagRemoved OnGameplayTagRemoved;

	UCActionComponent();
	
	UFUNCTION(BlueprintCallable, Category="Actions", meta=(DisplayName = "Get ActionComponent From Actor", Tooltip = "Returns ActionComponent if the Actor has any. Otherwise a nullptr."))
	static UCActionComponent* GetComponentFrom(AActor* FromActor);

	UFUNCTION(BlueprintCallable, Category="Actions")
	void AddAction(TSubclassOf<UCBaseAction> ActionClass, AActor* Instigator);

	UFUNCTION(BlueprintCallable, Category="Actions")
	void RemoveAction(UCBaseAction* ActionToRemove, AActor* Instigator);

	UFUNCTION(BlueprintCallable, Category="Actions")
	bool StartActionByTag(AActor* Instigator, FGameplayTag Tag);

	UFUNCTION(BlueprintCallable, Category="Actions")
	bool StopActionByTag(AActor* Instigator, FGameplayTag Tag);

	UFUNCTION(BlueprintCallable, Category="Tags", meta=(Tooltip = "Should be used for adding/removing keycard-style GameplayTags."))
	void AddGameplayTag(FGameplayTag Tag);

	UFUNCTION(BlueprintCallable, Category="Tags", meta=(Tooltip = "Should be used for adding/removing keycard-style GameplayTags."))
	bool RemoveGameplayTag(FGameplayTag Tag);

private:
	UPROPERTY(EditAnywhere, Category="Actions")
	TArray<TSubclassOf<UCBaseAction>> DefaultActions;

	UFUNCTION(Server, Reliable)
	void ServerStartAction(AActor* Instigator, FGameplayTag Tag);
	
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
};
