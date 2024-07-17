#include "Framework/CPlayerController.h"

void ACPlayerController::BeginPlayingState()
{
	Super::BeginPlayingState();

	BlueprintBeginPlayingState();
	OnBeginPlayStateStarted.Broadcast(this);
}
