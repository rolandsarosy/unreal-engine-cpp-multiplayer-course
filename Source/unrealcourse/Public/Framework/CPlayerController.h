#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "GameFramework/PlayerController.h"
#include "CPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBeginPlayingStateStarted, ACPlayerController*, PlayerController);

UCLASS()
class UNREALCOURSE_API ACPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void TogglePauseMenu();

protected:
	UPROPERTY(BlueprintAssignable, Category="PlayerController")
	FOnBeginPlayingStateStarted OnBeginPlayStateStarted;

	UPROPERTY(EditDefaultsOnly, Category="UI")
	TSubclassOf<UUserWidget> PauseMenuClass;

	UPROPERTY()
	UUserWidget* PauseMenuInstance;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> Input_PauseMenu;

	UFUNCTION(BlueprintImplementableEvent)
	void BlueprintBeginPlayingState();

private:
	/**
	* Called when player controller is ready to begin playing, good moment to initialize things like UI which might be too early in BeginPlay
	* (especially in multiplayer clients where not all data such as PlayerState may have been received yet)
	 */
	virtual void BeginPlayingState() override;

	virtual void SetupInputComponent() override;
};
