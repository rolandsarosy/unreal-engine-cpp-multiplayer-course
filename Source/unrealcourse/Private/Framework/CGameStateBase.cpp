#include "Framework/CGameStateBase.h"

void ACGameStateBase::ChangeGamePausedState(const bool bShouldBePaused)
{ 
	if (!ensure(HasAuthority())) return; // Should only be called by the server.

	MulticastOnGamePauseStateChanged(bShouldBePaused);
}

void ACGameStateBase::MulticastOnGamePauseStateChanged_Implementation(const bool bShouldBePaused)
{
	OnGamePauseStateChanged.Broadcast(bShouldBePaused);
}
