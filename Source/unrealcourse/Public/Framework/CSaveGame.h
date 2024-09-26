#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "CSaveGame.generated.h"

USTRUCT()
struct FActorSaveData
{
	GENERATED_BODY()

	/* Identifier for which Actor this belongs to. */
	UPROPERTY()
	FString ActorName;

	/* For movable Actors, keep location, rotation and scale. */
	UPROPERTY()
	FTransform Transform;

	/* Contains serialized information about properties marked with UPROPERTY(SaveGame) */
	UPROPERTY()
	TArray<uint8> ByteData;
};

UCLASS()
class UNREALCOURSE_API UCSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY()
	bool bIsInitialLoading = true;
	
	UPROPERTY()
	int32 Coins;

	UPROPERTY()
	TArray<FActorSaveData> SavedActors;
};
