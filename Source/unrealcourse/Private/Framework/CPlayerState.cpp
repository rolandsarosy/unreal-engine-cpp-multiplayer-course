#include "Framework/CPlayerState.h"

#include "Net/UnrealNetwork.h"

ACPlayerState::ACPlayerState()
{
	CurrentCoinsAmount = 10;
}

int32 ACPlayerState::GetCoinsAmount() const { return CurrentCoinsAmount; }

bool ACPlayerState::AddCoins(AActor* InstigatorActor, const int32 AmountToAdd)
{
	if (CurrentCoinsAmount + AmountToAdd < MAX_int32)
	{
		if (GetOwner()->HasAuthority()) // Coin calculations should only be done on the server, but generally knowing coin events should be known by clients as well.
		{
			CurrentCoinsAmount += AmountToAdd;
			MulticastCoinsAmountChange(InstigatorActor, CurrentCoinsAmount, AmountToAdd);
		}
		return true;
	}

	return false;
}

bool ACPlayerState::RemoveCoins(AActor* InstigatorActor, const int32 AmountToRemove)
{
	if (AmountToRemove <= CurrentCoinsAmount)
	{
		if (GetOwner()->HasAuthority()) // Coin calculations should only be done on the server, but generally knowing coin events should be known by clients as well.
		{
			CurrentCoinsAmount -= AmountToRemove;
			MulticastCoinsAmountChange(InstigatorActor, CurrentCoinsAmount, AmountToRemove);
		}
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

void ACPlayerState::MulticastCoinsAmountChange_Implementation(AActor* InstigatorActor, int32 NewCoinsAmount, int32 Delta)
{
	OnCoinsAmountChanged.Broadcast(InstigatorActor, CurrentCoinsAmount, Delta);
}

void ACPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACPlayerState, CurrentCoinsAmount);
}
