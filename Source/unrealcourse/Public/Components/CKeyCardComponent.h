#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "CKeyCardComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameplayTagAdded, FGameplayTag, TagAdded);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameplayTagRemoved, FGameplayTag, TagRemoved);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UNREALCOURSE_API UCKeyCardComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCKeyCardComponent();

	UPROPERTY(ReplicatedUsing="OnRep_CurrentKeycardGameplayTags", VisibleAnywhere, BlueprintReadOnly, Category="Keycards")
	FGameplayTagContainer CurrentKeycardGameplayTags;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Keycards")
	FGameplayTagContainer DefaultKeycardGameplayTags;

	UPROPERTY(BlueprintAssignable, Category="Keycards")
	FOnGameplayTagAdded OnKeycardGameplayTagAdded;

	UPROPERTY(BlueprintAssignable, Category="Keycards")
	FOnGameplayTagRemoved OnKeycardGameplayTagRemoved;

	UFUNCTION(Server, Reliable, BlueprintCallable, Category="Keycards", meta=(Tooltip = "Should be used for adding keycard-style GameplayTags."))
	void AddKeycardGameplayTag(const FGameplayTag Tag);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category="Keycards", meta=(Tooltip = "Should be used for removing keycard-style GameplayTags."))
	void RemoveKeycardGameplayTag(const FGameplayTag Tag);

	UFUNCTION(BlueprintCallable, Category="Keycards", meta=(DisplayName = "Get KeycardCompoent From Actor", Tooltip = "Returns the KeycardCompoent if the Actor has any. Otherwise a nullptr."))
	static UCKeyCardComponent* GetComponentFrom(AActor* FromActor);

private:
	UPROPERTY(VisibleAnywhere, Category="Keycards")
	FGameplayTagContainer PreviousKeycardGameplayTags; // Stores the list of actions before an OnRep triggers, as OnRep cannot carry the previous value.

	UFUNCTION()
	void OnRep_CurrentKeycardGameplayTags();
	
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
