#include "CPlayerState.h"

ACPlayerState::ACPlayerState()
{
	CurrentCoinsAmount = 10;
}

int32 ACPlayerState::GetCoinsAmount() const { return CurrentCoinsAmount; }

bool ACPlayerState::AddCoins(AActor* InstigatorActor, const int32 AmountToAdd)
{
	if (AmountToAdd < 1) return false;
	
	if (CurrentCoinsAmount + AmountToAdd < MAX_int32)
	{
		CurrentCoinsAmount += AmountToAdd;
		OnCoinsAmountChanged.Broadcast(InstigatorActor, CurrentCoinsAmount, AmountToAdd);
		return true;
	}

	return false;
}

bool ACPlayerState::RemoveCoins(AActor* InstigatorActor, const int32 AmountToRemove)
{
	if (AmountToRemove > -1) return false;
	
	if (AmountToRemove <= CurrentCoinsAmount)
	{
		CurrentCoinsAmount -= AmountToRemove;
		OnCoinsAmountChanged.Broadcast(InstigatorActor, CurrentCoinsAmount, AmountToRemove);
		return true;
	}

	return false;
}

ACPlayerState* ACPlayerState::GetFromActor(AActor* FromActor)
{
	const APawn* Pawn = Cast<APawn>(FromActor);
	if (!Pawn) return nullptr;

	return Cast<ACPlayerState>(Pawn->GetPlayerState());
}
