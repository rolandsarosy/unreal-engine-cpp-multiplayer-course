#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "CPickupSpawnerComponent.generated.h"

class UEnvQueryInstanceBlueprintWrapper;
class ACBasePickup;
class UEnvQuery;

USTRUCT()
struct FCPickupItemSpawnInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, Category="Spawner Info")
	TSubclassOf<ACBasePickup> ItemClass;

	UPROPERTY(EditDefaultsOnly, Category="Spawner Info")
	int32 SpawnCount;
};

UCLASS(ClassGroup=(Custom))
class UNREALCOURSE_API UCPickupSpawnerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	void SpawnItems();

private:
	UPROPERTY(EditDefaultsOnly, Category="Spawner Info")
	TObjectPtr<UEnvQuery> SpawnPickupsEQ;

	UPROPERTY(EditAnywhere, Category="Spawner Info")
	TArray<FCPickupItemSpawnInfo> PickupItemsToSpawn;

	UFUNCTION()
	void OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	void SpawnPickupItemsToQueryLocations(TArray<FVector>& SpawnLocations);

	bool GetSpawnPointsAmountValidity(const TArray<FVector>& SpawnLocations);
};
