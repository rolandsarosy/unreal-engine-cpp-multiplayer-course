#include "Framework/CPlayerController.h"

#include "EnhancedInputComponent.h"
#include "Blueprint/UserWidget.h"

void ACPlayerController::TogglePauseMenu()
{
	// Pause menu is currently visible, remove it.
	if (PauseMenuInstance && PauseMenuInstance->IsInViewport())
	{
		PauseMenuInstance->RemoveFromParent();
		PauseMenuInstance = nullptr;
		bShowMouseCursor = false;
		SetInputMode(FInputModeGameOnly());
		
		return;
	}

	// Pause menu is currently not visible, add it.
	if (PauseMenuInstance = CreateWidget<UUserWidget>(this, PauseMenuClass); PauseMenuInstance)
	{
		PauseMenuInstance->AddToViewport(100);
		bShowMouseCursor = true;
		SetInputMode(FInputModeUIOnly());
	}
}

void ACPlayerController::BeginPlayingState()
{
	Super::BeginPlayingState();

	BlueprintBeginPlayingState();
	OnBeginPlayStateStarted.Broadcast(this);
}

void ACPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(Input_PauseMenu, ETriggerEvent::Triggered, this, &ACPlayerController::TogglePauseMenu);
	}
}
