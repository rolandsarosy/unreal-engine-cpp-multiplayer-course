#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CSaveGameSubsystem.generated.h"

class UCSaveGame;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSaveGameWritten, UCSaveGame*, SaveObject);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSaveGameLoaded, UCSaveGame*, SaveObject);

UCLASS(meta=(DisplayName="SaveGame Subsystem"))
class UNREALCOURSE_API UCSaveGameSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnSaveGameWritten OnSaveGameWritten;

	UPROPERTY(BlueprintAssignable)
	FOnSaveGameLoaded OnSaveGameLoaded;
	
	UFUNCTION(BlueprintCallable, Category="SaveGame")
	void WriteSaveGame();
	
	void HandleStartingNewPlayer(const AController* NewPlayer) const;

	void LoadSaveGame(FString InSlotName = "");
	
private:
	UPROPERTY()
	TObjectPtr<UCSaveGame> CurrentSaveGame;

	FString CurrentSaveSlotName;
	
	UFUNCTION()
	void SetSaveSlotName(FString NewSaveSlotName);

	/* Initialize Subsystem, good moment to load in SaveGameSettings variables */
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
};
