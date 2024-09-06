#include "Framework/CPlayerController.h"

#include "EnhancedInputComponent.h"
#include "Blueprint/UserWidget.h"
#include "Framework/CGameStateBase.h"

void ACPlayerController::BeginPlay()
{
	Super::BeginPlay();
	Cast<ACGameStateBase>(GetWorld()->GetGameState())->OnGamePauseStateChanged.AddDynamic(this, &ACPlayerController::OnGamePauseStateChanged);
}

void ACPlayerController::BeginPlayingState()
{
	Super::BeginPlayingState();

	BlueprintBeginPlayingState();
	OnBeginPlayStateStarted.Broadcast(this);
}

void ACPlayerController::ServerToggleGamePause_Implementation()
{
	SetPause(!IsPaused());
}

void ACPlayerController::OnGamePauseStateChanged(const bool bIsPaused)
{
	if (!IsLocalController()) return;

	if (bIsPaused)
	{
		PauseMenuInstance = CreateWidget<UUserWidget>(this, PauseMenuClass);
		PauseMenuInstance->AddToViewport(100);
		bShowMouseCursor = true;
		SetInputMode(FInputModeUIOnly());
	}
	else
	{
		PauseMenuInstance->RemoveFromParent();
		PauseMenuInstance = nullptr;
		bShowMouseCursor = false;
		SetInputMode(FInputModeGameOnly());
	}
}

void ACPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(Input_PauseMenu, ETriggerEvent::Triggered, this, &ACPlayerController::ServerToggleGamePause);
	}
}
