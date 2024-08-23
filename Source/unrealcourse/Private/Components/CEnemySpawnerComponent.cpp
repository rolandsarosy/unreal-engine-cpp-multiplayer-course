#include "Components/CEnemySpawnerComponent.h"

#include "EngineUtils.h"
#include "AI/CAICharacter.h"
#include "Algo/RandomShuffle.h"
#include "Components/CAttributeComponent.h"
#include "EnvironmentQuery/EnvQueryManager.h"

static TAutoConsoleVariable CVarSpawnEnemies(TEXT("course.SpawnEnemies"), true, TEXT("Enabled or disables spawning of enemies via timers."), ECVF_Cheat);

UCEnemySpawnerComponent::UCEnemySpawnerComponent()
{
	EnemySpawnTimerInterval = 2.0f;
}

void UCEnemySpawnerComponent::BeginPlay()
{
	Super::BeginPlay();
	FTimerHandle TimerHandle_SpawnEnemies;
	GetOwner()->GetWorldTimerManager().SetTimer(TimerHandle_SpawnEnemies, this, &UCEnemySpawnerComponent::OnSpawnEnemyTimerElapsed, EnemySpawnTimerInterval, true);
}

void UCEnemySpawnerComponent::OnSpawnEnemyTimerElapsed()
{
	if (!CVarSpawnEnemies.GetValueOnGameThread())
	{
		UE_LOG(LogTemp, Warning, TEXT("Enemy Spawn"))
		return;
	}

	if (CanGameModeSpawnMoreEnemies(GetNumberOfEnemiesAlive()))
	{
		UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(GetWorld(), SpawnEnemyEQ, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);

		if (!ensure(QueryInstance)) return;
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &UCEnemySpawnerComponent::OnQueryCompleted);
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst ~ Incorrect suggestion
// ReSharper disable once CppParameterMayBeConstPtrOrRef ~ Incorrect suggestion
void UCEnemySpawnerComponent::OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	switch (QueryStatus)
	{
	case EEnvQueryStatus::Success:
		if (QueryInstance->GetResultsAsLocations().IsValidIndex(0)) SpawnEnemyAtLocation(QueryInstance->GetResultsAsLocations()[0]);
	case EEnvQueryStatus::Processing:
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("Enemy spawn EQ did not succeed."));
	}
}

void UCEnemySpawnerComponent::SpawnEnemyAtLocation(const FVector& SpawnLocation) const
{
	const TSubclassOf<ACAICharacter> EnemyTypeToSpawn = GetEnemyTypeToSpawn();
	if (!ensureMsgf(EnemyTypeToSpawn, TEXT("Was unable to determine enemy type to spawn from enemy data table."))) return;

	const AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(EnemyTypeToSpawn, SpawnLocation, FRotator::ZeroRotator);

#if WITH_EDITOR
	UE_LOG(LogTemp, Log, TEXT("%s attempted to spawn enemy class of %s at %s. Return value was: %p"),
	       *GetNameSafe(this), *GetNameSafe(EnemyTypeToSpawn), *SpawnLocation.ToString(), *GetNameSafe(SpawnedActor));

	if (SpawnedActor)
	{
		DrawDebugSphere(GetWorld(), SpawnLocation, 15.0f, 16, FColor::Yellow, false, 5.0f, 0, 1);
		DrawDebugString(GetWorld(), SpawnLocation, TEXT("Enemy Spawned Here"), nullptr, FColor::Yellow, 5.0f, true);
	}
#endif
}

// TODO - This is a computationally expensive call. It was written during class, but I'd like to write a more efficient solution later on.
// ReSharper disable once CppTooWideScopeInitStatement ~ Results in worse readability
uint16 UCEnemySpawnerComponent::GetNumberOfEnemiesAlive() const
{
	uint16 NumberOfEnemiesAlive = 0;
	for (TActorIterator<ACAICharacter> Iterator(GetWorld()); Iterator; ++Iterator)
	{
		ACAICharacter* Enemy = *Iterator;

		const UCAttributeComponent* AttributeComponent = UCAttributeComponent::GetComponentFrom(Enemy);
		if (AttributeComponent && AttributeComponent->IsAlive()) NumberOfEnemiesAlive++;
	}

	return NumberOfEnemiesAlive;
}

/**
 * @brief Retrieves the type of enemy to spawn based on the enemy data table, respecting the spawn weights.
 *
 * @return The subclass of ACAICharacter representing the enemy type to spawn, or nullptr if no suitable enemy type was found.
 */
TSubclassOf<ACAICharacter> UCEnemySpawnerComponent::GetEnemyTypeToSpawn() const
{
	// Automatically reject enemy spawns if the enemy data table hasn't been set.
	if (!ensureMsgf(EnemyTable, TEXT("Enemy data table must be set to be able to spawn enemies."))) return nullptr;

	TArray<FEnemyInfoRow*> EnemyRows;
	EnemyTable->GetAllRows("EnemyTableRowRequest", EnemyRows);

	float TotalWeight = 0;
	for (const auto& EnemyRow : EnemyRows)
	{
		TotalWeight += EnemyRow->SpawnWeight;
	}
	
	Algo::RandomShuffle(EnemyRows);
	
	float RandomWeight = FMath::FRand() * TotalWeight;
	for (const auto& EnemyRow : EnemyRows)
	{
		RandomWeight -= EnemyRow->SpawnWeight;
		if (RandomWeight <= 0) { return EnemyRow->SpawnEnemyClass; }
	}

	return nullptr;
}

/**
 * @brief Checks if the game mode is able to spawn more enemies.
 *
 * @param NumberOfEnemiesAlive The number of enemies currently alive.
 *
 * @return True if the game mode can spawn more enemies, false otherwise.
 */
bool UCEnemySpawnerComponent::CanGameModeSpawnMoreEnemies(const uint16 NumberOfEnemiesAlive) const
{
	// Automatically reject enemy spawns if the difficulty curve hasn't been set.
	if (!ensureMsgf(MaxEnemyCountOverTimeCurve, TEXT("Difficulty curve asset must be set for GameMode."))) return false;

	const uint16 MaxNumberOfEnemiesAlive = MaxEnemyCountOverTimeCurve->GetFloatValue(GetWorld()->TimeSeconds);
	return NumberOfEnemiesAlive < MaxNumberOfEnemiesAlive;
}
