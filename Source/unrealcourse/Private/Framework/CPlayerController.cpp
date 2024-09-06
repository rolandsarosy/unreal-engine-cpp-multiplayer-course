#include "Framework/CPlayerController.h"

#include "EnhancedInputComponent.h"
#include "Blueprint/UserWidget.h"
#include "Framework/CGameStateBase.h"
#include "unrealcourse/unrealcourse.h"

void ACPlayerController::ServerToggleGamePause_Implementation()
{
	const bool bIsCurrentlyPaused = IsPaused(); // Just putting this into a variable for the time being to make sure we don't run into race condition issues. @FIXME before pushing.
	SetPause(!bIsCurrentlyPaused);
}

void ACPlayerController::OnGamePauseStateChanged(bool bIsPaused)
{
	if (!IsLocalController()) return;

	LogOnScreen(GetWorld(), FString::Printf(TEXT("OnGamePauseStateChanged delegate triggered on local PlayerController.")));
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

void ACPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	LogOnScreen(GetWorld(), FString::Printf(TEXT("Starting to listen to delegate.")));
	ACGameStateBase* GameState = Cast<ACGameStateBase>(GetWorld()->GetGameState());
	GameState->OnGamePauseStateChanged.AddDynamic(this, &ACPlayerController::OnGamePauseStateChanged);
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
		EnhancedInputComponent->BindAction(Input_PauseMenu, ETriggerEvent::Triggered, this, &ACPlayerController::ServerToggleGamePause);
	}
}
