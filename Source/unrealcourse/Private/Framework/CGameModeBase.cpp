#include "Framework/CGameModeBase.h"

#include "Framework/CSaveGame.h"
#include "EngineUtils.h"
#include "Framework/CCharacter.h"
#include "Framework/CPlayerState.h"
#include "AI/CAICharacter.h"
#include "Components/CEnemySpawnerComponent.h"
#include "Components/CPickupSpawnerComponent.h"
#include "Framework/CGameStateBase.h"
#include "GameFramework/GameStateBase.h"
#include "Interfaces/CSaveableInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

ACGameModeBase::ACGameModeBase()
{
	PickupSpawnerComponent = CreateDefaultSubobject<UCPickupSpawnerComponent>("PickupSpawnerComponent");
	EnemySpawnerComponent = CreateDefaultSubobject<UCEnemySpawnerComponent>("EnemySpawnerComponent");

	PlayerRespawnDelay = 5.0f;

	SaveGameSlotName = "SaveGame01";
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
	LoadSaveGameFromDisk();
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
	// TODO - Currently this causes an issue where the initial PlayerState Coin values are disregarded on the initial SaveState being created. This'll be resolved with Composite DataTables later on.
	if (ACPlayerState* PlayerState = NewPlayer->GetPlayerState<ACPlayerState>()) { PlayerState->LoadPlayerState(CurrentSaveGame); }

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
		Cast<ACPlayerState>(Cast<APawn>(Killer)->GetPlayerState())->AddCoins(Victim, Cast<ACAICharacter>(Victim)->CoinRewardUponDeath);
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

/**
 * @brief Writes the current game state to a save file on disk.
 *
 * This method iterates through all Players and saves their state to the current save game object.
 * Also iterates through all actors and if they implement the UCGameplayInterface, the actor's state is serialized and added to the list of saved actors in the save game object.
 * The save game object is then saved to a specified slot using SaveGameToSlot() function from UGameplayStatics class.
 *
 * @warning This method assumes single-player behavior only for now. 
 * @warning This method does not save the level name along with actor data for now.
 *
 */
void ACGameModeBase::WriteSaveGameToDisk() const
{
	// Iterate through all PlayerState(s), we don't have a proper ID to match yet. (Requires Steam or EOS)
	for (int32 i = 0; i < GameState->PlayerArray.Num(); i++)
	{
		if (ACPlayerState* PlayerState = Cast<ACPlayerState>(GameState->PlayerArray[i]))
		{
			PlayerState->SavePlayerState(CurrentSaveGame);
			break; // Single-player behavior only for the time being. TODO: Make this multiplayer-compatible by Saving Credits to a persisent UUID like a Steam Account ID.
		}
	}

	// Clear list of previous items
	CurrentSaveGame->SavedActors.Empty();

	// Iterate through all Actors in the World.
	for (FActorIterator It(GetWorld()); It; ++It)
	{
		AActor* Actor = *It;
		if (!Actor->Implements<UCSaveableInterface>()) { continue; } // Don't bother with items not implementing this interface.

		FActorSaveData ActorData;
		ActorData.ActorName = Actor->GetName(); // Single-level saving behavior for the time being only. TODO: Save level name along with Actor data. Eg.: Level_01_TreasureChest2
		ActorData.Transform = Actor->GetActorTransform();

		FMemoryWriter MemoryWriter(ActorData.ByteData);
		FObjectAndNameAsStringProxyArchive Archive(MemoryWriter, true);
		Archive.ArIsSaveGame = true; // Find only variables with UPROPERTY(SaveGame) tags.

		Actor->Serialize(Archive);

		CurrentSaveGame->SavedActors.Add(ActorData);
	}

	UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SaveGameSlotName, 0);
}

/**
 * @brief Loads the SaveGame data from the disk.
 *
 * Checks if a save game of a given name exists and loads serialized actors' world data and serialized byte data into the actors themselves, and calls the actors' OnActorLoaded interface function.
 * Creates the new save if it did not exist yet.
 */
void ACGameModeBase::LoadSaveGameFromDisk()
{
	if (UGameplayStatics::DoesSaveGameExist(SaveGameSlotName, 0))
	{
		CurrentSaveGame = Cast<UCSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveGameSlotName, 0));
		if (CurrentSaveGame == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to load SaveGame data."))
			return;
		}

		UE_LOG(LogTemp, Warning, TEXT("Successfully loaded SaveGame data."))

		// Iterate through all Actors in the World. Not applicable if a new SaveGame was just created.
		for (FActorIterator It(GetWorld()); It; ++It)
		{
			AActor* Actor = *It;
			if (!Actor->Implements<UCSaveableInterface>()) { continue; } // Don't bother with items not implementing this interface.

			for (auto [ActorName, Transform, ByteData] : CurrentSaveGame->SavedActors)
			{
				if (ActorName == Actor->GetName())
				{
					Actor->SetActorTransform(Transform);

					FMemoryReader MemoryReader(ByteData);
					FObjectAndNameAsStringProxyArchive Archive(MemoryReader, true);
					Archive.ArIsSaveGame = true;
					Actor->Serialize(Archive);

					ICSaveableInterface::Execute_OnActorLoaded(Actor);

					break;
				}
			}
		}
	}
	else
	{
		CurrentSaveGame = Cast<UCSaveGame>(UGameplayStatics::CreateSaveGameObject(UCSaveGame::StaticClass()));
		UE_LOG(LogTemp, Warning, TEXT("Successfully created new transient SaveGame data."))
	}
}
