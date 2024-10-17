#include "CSaveGameSubsystem.h"

#include "CSaveGameSettings.h"
#include "EngineUtils.h"
#include "Framework/CGameStateBase.h"
#include "Framework/CPlayerState.h"
#include "Framework/CSaveGame.h"
#include "Interfaces/CSaveableInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

void UCSaveGameSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	const UCSaveGameSettings* SaveGameSettings = GetDefault<UCSaveGameSettings>();
	CurrentSaveSlotName = SaveGameSettings->SaveSlotName;
}

void UCSaveGameSubsystem::HandleStartingNewPlayer(const AController* NewPlayer) const
{
	ACPlayerState* PlayerState = NewPlayer->GetPlayerState<ACPlayerState>();
	if (!ensure(PlayerState)) return;

	// Use the PlayerState's defaults for initial loading.
	if (CurrentSaveGame->bIsInitialLoading)
	{
		CurrentSaveGame->bIsInitialLoading = false;
	}
	else
	{
		PlayerState->LoadPlayerState(CurrentSaveGame);
	}
}

/**
 * @brief Writes the current game state to a save file on disk.
 *
 * This method iterates through all Players and saves their state to the current save game object.
 * Also iterates through all actors and if they implement the UCGameplayInterface, the actor's state is serialized and added to the list of saved actors in the save game object.
 * The save game object is then saved to a specified slot using SaveGameToSlot() function from UGameplayStatics class.
 *
 * @warning This method assumes single-player behavior only for now. 
 */
void UCSaveGameSubsystem::WriteSaveGame()
{
	CurrentSaveGame->SavedActors.Empty();
	AGameStateBase* GameState = GetWorld()->GetGameState();

	// Iterate through all PlayerState(s) as we don't have a proper ID to match yet. (Requires Steam or EOS)
	for (int32 i = 0; i < GameState->PlayerArray.Num(); i++)
	{
		if (ACPlayerState* PlayerState = Cast<ACPlayerState>(GameState->PlayerArray[i]))
		{
			PlayerState->SavePlayerState(CurrentSaveGame);
			break; // Single-player behavior only for the time being. TODO: Make this multiplayer-compatible by Saving Credits to a persisent UUID like a Steam Account ID.
		}
	}

	// Iterate through all Actors in the World.
	for (AActor* Actor : TActorRange<AActor>(GetWorld()))
	{
		if (!IsValid(Actor) || !Actor->Implements<UCSaveableInterface>()) { continue; } // Don't bother with items not implementing this interface or with those that are being deleted.

		FActorSaveData ActorData;
		ActorData.ActorName = Actor->GetLevel()->GetName() + Actor->GetName();
		ActorData.Transform = Actor->GetActorTransform();

		FMemoryWriter MemoryWriter(ActorData.ByteData);
		FObjectAndNameAsStringProxyArchive Archive(MemoryWriter, true);
		Archive.ArIsSaveGame = true; // Find only variables with UPROPERTY(SaveGame) tags.

		Actor->Serialize(Archive);

		CurrentSaveGame->SavedActors.Add(ActorData);
	}

	UGameplayStatics::SaveGameToSlot(CurrentSaveGame, CurrentSaveSlotName, 0);
	OnSaveGameWritten.Broadcast(CurrentSaveGame);
}

/**
 * @brief Loads the SaveGame data from the disk.
 *
 * Checks if a save game of a given name exists and loads serialized actors' world data and serialized byte data into the actors themselves, and calls the actors' OnActorLoaded interface function.
 * Creates the new save if it did not exist yet.
 */
void UCSaveGameSubsystem::LoadSaveGame(FString InSlotName)
{
	SetSaveSlotName(InSlotName);
	
	if (UGameplayStatics::DoesSaveGameExist(CurrentSaveSlotName, 0))
	{
		CurrentSaveGame = Cast<UCSaveGame>(UGameplayStatics::LoadGameFromSlot(CurrentSaveSlotName, 0));
		if (CurrentSaveGame == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to load SaveGame data."))
			return;
		}

		UE_LOG(LogTemp, Log, TEXT("Successfully loaded SaveGame data."))
		
		// Iterate through all Actors in the World. Not applicable if a new SaveGame was just created.
		for (AActor* Actor : TActorRange<AActor>(GetWorld()))
		{
			if (!Actor->Implements<UCSaveableInterface>()) { continue; } // Don't bother with items not implementing this interface.

			for (auto [ActorName, Transform, ByteData] : CurrentSaveGame->SavedActors)
			{
				if (ActorName == Actor->GetLevel()->GetName() + Actor->GetName())
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

		OnSaveGameLoaded.Broadcast(CurrentSaveGame);
	}
	else
	{
		CurrentSaveGame = Cast<UCSaveGame>(UGameplayStatics::CreateSaveGameObject(UCSaveGame::StaticClass()));
		UE_LOG(LogTemp, Log, TEXT("Successfully created new transient SaveGame data."))
	}
}

void UCSaveGameSubsystem::SetSaveSlotName(FString NewSaveSlotName)
{
	if (NewSaveSlotName.IsEmpty()) return;
	CurrentSaveSlotName = NewSaveSlotName;
}
