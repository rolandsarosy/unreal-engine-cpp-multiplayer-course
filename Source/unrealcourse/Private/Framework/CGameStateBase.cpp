#include "Framework/CGameStateBase.h"

#include "Net/UnrealNetwork.h"
#include "unrealcourse/unrealcourse.h"

ACGameStateBase::ACGameStateBase()
{
	bIsGamePaused = false;
}

void ACGameStateBase::ChangeGamePausedState(const bool bShouldBePaused)
{ 
	if (!ensure(HasAuthority())) return; // Should only be called by the server.
	
	bIsGamePaused = bShouldBePaused;
	OnRep_IsGamePaused(); // OnRep_Foo() functions don't trigger automatically on the server. Clients will ignore the double call of the function as they already have the correct state set.
}

void ACGameStateBase::OnRep_IsGamePaused() const
{
	LogOnScreen(GetWorld(), FString::Printf(TEXT("OnRep for IsGamePaused triggered.")));
	OnGamePauseStateChanged.Broadcast(bIsGamePaused);
}

void ACGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACGameStateBase, bIsGamePaused);
}
