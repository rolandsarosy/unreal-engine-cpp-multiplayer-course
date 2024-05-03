#include "CGameModeBase.h"

#include "CCharacter.h"
#include "CPlayerState.h"
#include "AI/CAICharacter.h"
#include "Components/CEnemySpawnerComponent.h"
#include "Components/CPickupSpawnerComponent.h"

ACGameModeBase::ACGameModeBase()
{
	PickupSpawnerComponent = CreateDefaultSubobject<UCPickupSpawnerComponent>("PickupSpawnerComponent");
	EnemySpawnerComponent = CreateDefaultSubobject<UCEnemySpawnerComponent>("EnemySpawnerComponent");

	PlayerRespawnDelay = 5.0f;
}

/**
 * @brief Called when an actor is killed.
 *
 * @note This method is usually directly called from the Victim's @UCAttributeComponent.
 *
 * @param Victim The actor that was killed.
 * @param Killer The actor that killed the victim.
 */
void ACGameModeBase::OnActorKilled(AActor* Victim, AActor* Killer)
{
	UE_LOG(LogTemp, Log, TEXT("OnActorKilled: Victim: %s, Killer: %s"), *GetNameSafe(Victim), *GetNameSafe(Killer));

	// Check if the Victim is the Player and start respawn timer
	if (ACCharacter* PlayerCharacter = Cast<ACCharacter>(Victim))
	{
		FTimerHandle TimerHandle_RespawnDelay;
		GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay, [this, PlayerCharacter] { RespawnPlayer(PlayerCharacter->GetController()); }, PlayerRespawnDelay, false);
		return;
	}

	// Check if the Victim is an enemy and reward coins
	if (Victim->IsA(ACAICharacter::StaticClass()) && Killer->IsA(ACCharacter::StaticClass()))
	{
		Cast<ACPlayerState>(Cast<APawn>(Killer)->GetPlayerState())->AddCoins(Victim, Cast<ACAICharacter>(Victim)->CoinRewardUponDeath);
		return;
	}
}

void ACGameModeBase::RespawnPlayer(AController* PlayerController)
{
	if (!ensureMsgf(PlayerController, TEXT("Was unable to respawn player as the Controller was invalid."))) return;

	PlayerController->UnPossess();
	RestartPlayer(PlayerController);
}
