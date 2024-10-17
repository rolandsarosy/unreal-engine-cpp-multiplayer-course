#include "Framework/CGameModeBase.h"

#include "CSaveGameSubsystem.h"
#include "Framework/CCharacter.h"
#include "Framework/CPlayerState.h"
#include "AI/CAICharacter.h"
#include "Components/CEnemySpawnerComponent.h"
#include "Components/CPickupSpawnerComponent.h"
#include "Framework/CGameStateBase.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"

ACGameModeBase::ACGameModeBase()
{
	PickupSpawnerComponent = CreateDefaultSubobject<UCPickupSpawnerComponent>("PickupSpawnerComponent");
	EnemySpawnerComponent = CreateDefaultSubobject<UCEnemySpawnerComponent>("EnemySpawnerComponent");

	PlayerRespawnDelay = 5.0f;
}

/**
 * @brief Initialize the game.
 * The GameMode's InitGame() event is called before any other functions (including PreInitializeComponents()) and is used by the GameMode to initialize parameters and spawn its helper classes.
 *
 * @warning Called before actors' PreInitializeComponents.
 */
void ACGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	GetGameInstance()->GetSubsystem<UCSaveGameSubsystem>()->LoadSaveGame(UGameplayStatics::ParseOption(Options, "SaveSlotName"));
}

/**
 * 
 * @brief Called when a player is about to enter the game.
 *
 * Used to initilalize the PlayerState from serialized data, if there is any.
 *
 * @param NewPlayer The [PlayerController] joining the game.
 */
void ACGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	GetGameInstance()->GetSubsystem<UCSaveGameSubsystem>()->HandleStartingNewPlayer(NewPlayer);
	
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);
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
		Cast<ACPlayerState>(Cast<APawn>(Killer)->GetPlayerState())->AddCoins(Victim, Cast<ACAICharacter>(Victim)->GetCoinRewardUponDeath());
	}
}

void ACGameModeBase::RespawnPlayer(AController* PlayerController)
{
	if (!ensureMsgf(PlayerController, TEXT("Was unable to respawn player as the Controller was invalid."))) return;

	PlayerController->UnPossess();
	RestartPlayer(PlayerController);
}

bool ACGameModeBase::SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate)
{
	const bool bCouldPause = Super::SetPause(PC, CanUnpauseDelegate);
	Cast<ACGameStateBase>(GameState)->ChangeGamePausedState(bCouldPause);

	if (!bCouldPause) UE_LOG(LogTemp, Error, TEXT("GameMode was unable to pause the game."))

	return bCouldPause;
}

bool ACGameModeBase::ClearPause()
{
	const bool bCouldClearPause = Super::ClearPause();
	Cast<ACGameStateBase>(GameState)->ChangeGamePausedState(!bCouldClearPause);

	return bCouldClearPause;
}
