#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "CPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCoinsAmountChanged, AActor*, InstigatorActor, int32, NewCoinsAmount, int32, Delta);

class UCSaveGame;

UCLASS()
class UNREALCOURSE_API ACPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ACPlayerState();

	UPROPERTY(BlueprintAssignable, Category="Coins")
	FOnCoinsAmountChanged OnCoinsAmountChanged;

	UFUNCTION(BlueprintCallable, Category="Coins", meta= (DisplayName = "Get Current Coins Amount"))
	int32 GetCoinsAmount() const;

	UFUNCTION(BlueprintCallable, Category="Coins", meta=(DisplayName = "Add Coins", Tooltip = "Returns true if the amoutn of Coins was added to the Player. Returns false otherwise."))
	bool AddCoins(AActor* InstigatorActor, int32 AmountToAdd);

	UFUNCTION(BlueprintCallable, Category="Coins", meta=(DisplayName = "Set Coins", Tooltip = "Directly sets the amount of coins held by the player to a given amount. Always succeeds."))
	void SetCoins(AActor* InstigatorActor, int32 CoinsAmount);

	UFUNCTION(BlueprintCallable, Category="Coins", meta=(DisplayName = "Remove Coins", Tooltip = "Returns true if the amount is present and Coins have been removed. Returns false otherwise."))
	bool RemoveCoins(AActor* InstigatorActor, int32 AmountToRemove);

	UFUNCTION()
	void SavePlayerState(UCSaveGame* SaveGameObject) const;

	UFUNCTION()
	void LoadPlayerState(const UCSaveGame* SaveGameObject);

	UFUNCTION(BlueprintCallable, Category="Attributes", meta=(DisplayName = "Get PlayerState from Actor", Tooltip = "Returns PlayerState if the Actor has any. Otherwise a nullptr."))
	static ACPlayerState* GetFromActor(AActor* FromActor);

private:
	UPROPERTY(Replicated)
	int32 CurrentCoinsAmount;

	UFUNCTION(Unreliable, NetMulticast)
	void MulticastCoinsAmountChange(AActor* InstigatorActor, int32 NewCoinsAmount, int32 Delta);
};
