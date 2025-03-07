#include "Components/CPickupSpawnerComponent.h"

#include "EnvironmentQuery/EnvQueryManager.h"
#include "Interactables/CBasePickup.h"

void UCPickupSpawnerComponent::BeginPlay()
{
	Super::BeginPlay();
	QueryItemSpawnPoints();
}

void UCPickupSpawnerComponent::QueryItemSpawnPoints()
{
	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(GetWorld(), SpawnPickupsEQ, this, EEnvQueryRunMode::AllMatching, nullptr);
	if (!ensure(QueryInstance)) return;

	QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &UCPickupSpawnerComponent::OnQueryCompleted);
}

// ReSharper disable once CppMemberFunctionMayBeStatic - Incorrect suggestion
// ReSharper disable once CppParameterMayBeConstPtrOrRef - Incorrect suggestion
void UCPickupSpawnerComponent::OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	TArray<FVector> OutResultLocations;
	switch (QueryStatus)
	{
	case EEnvQueryStatus::Success:
		QueryInstance->GetQueryResultsAsLocations(OutResultLocations); // Returns a sorted list where the best matches are placed first.
		SpawnPickupItemsToQueryLocations(OutResultLocations);
		break;
	case EEnvQueryStatus::Processing:
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("Spawn Pickups EQ did not succeed."));
	}
}

// ReSharper disable once CppTooWideScope - Results in worse readability
void UCPickupSpawnerComponent::SpawnPickupItemsToQueryLocations(TArray<FVector>& SpawnLocations)
{
	if (!GetSpawnPointsAmountValidity(SpawnLocations)) return;

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = GetOwner();

	int CurrentLocationIndex = 0;
	for (const auto& [Item, SpawnCount] : PickupItemsToSpawn)
	{
		for (int i = 0; i < SpawnCount; ++i)
		{
			const AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(Item, SpawnLocations[CurrentLocationIndex], FRotator::ZeroRotator, SpawnParameters);
			if (SpawnedActor) CurrentLocationIndex++;
		}
	}
}

/**
 * Check if the number of spawn points is valid based on the total spawn amount of pickup items.
 *
 * @note As long as we're taking all valid items from the query, this should be true most of the time, but this is necessary to be safe.
 *
 * @param SpawnLocations The array of spawn locations for the pickup items.
 *
 * @return True if there are enough spawn points for the items to be spawned, false otherwise.
 */
bool UCPickupSpawnerComponent::GetSpawnPointsAmountValidity(const TArray<FVector>& SpawnLocations)
{
	int32 TotalSpawnAmount = 0;
	for (const auto& [_, SpawnCount] : PickupItemsToSpawn)
	{
		TotalSpawnAmount += SpawnCount;
	}

	const bool bIsSpawnPointsAmountValid = SpawnLocations.Num() >= TotalSpawnAmount;
	if (!bIsSpawnPointsAmountValid) UE_LOG(LogTemp, Error, TEXT("Pickup Spawner was unable to generate enough points for the desired amount of pickups. None will be spawned."))

	return bIsSpawnPointsAmountValid;
}
