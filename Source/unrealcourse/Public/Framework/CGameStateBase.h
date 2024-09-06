#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "CGameStateBase.generated.h"

/** 
 * @brief Parameterized Multicast Delegate that responds to pausing and unpausing a session.
 *
 * State is changed in the GameMode's SetPause() & ClearPause() methods.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGamePauseStateChanged, bool, bIsPaused);

UCLASS()
class UNREALCOURSE_API ACGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
	ACGameStateBase();

	UPROPERTY(BlueprintAssignable, Category="GameState")
	FOnGamePauseStateChanged OnGamePauseStateChanged;

	UFUNCTION()
	void ChangeGamePausedState(bool bShouldBePaused);

private:
	UPROPERTY(ReplicatedUsing="OnRep_IsGamePaused", VisibleAnywhere)
	bool bIsGamePaused;

	UFUNCTION()
	void OnRep_IsGamePaused() const;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
