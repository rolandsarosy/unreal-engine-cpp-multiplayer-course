#include "Components/CEnemySpawnerComponent.h"

#include "CEnemyData.h"
#include "EngineUtils.h"
#include "AI/CAICharacter.h"
#include "Algo/RandomShuffle.h"
#include "Components/CActionComponent.h"
#include "Components/CAttributeComponent.h"
#include "Engine/AssetManager.h"
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
	if (!CVarSpawnEnemies.GetValueOnGameThread()) return;

	if (CanGameModeSpawnMoreEnemies(GetNumberOfEnemiesAlive()))
	{
		UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(GetWorld(), SpawnEnemyEQ, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);

		if (!ensure(QueryInstance)) return;
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &UCEnemySpawnerComponent::OnSpawnLocationQueryCompleted);
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst ~ Incorrect suggestion
// ReSharper disable once CppParameterMayBeConstPtrOrRef ~ Incorrect suggestion
void UCEnemySpawnerComponent::OnSpawnLocationQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	switch (QueryStatus)
	{
	case EEnvQueryStatus::Success:
		if (QueryInstance->GetResultsAsLocations().IsValidIndex(0)) GetAsyncRandomSpawnableEnemyData(QueryInstance->GetResultsAsLocations()[0]);
	case EEnvQueryStatus::Processing:
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("Enemy spawn EQ did not succeed."));
	}
}

/**
 * @brief Retrieves the type of enemy to spawn based on the enemy data table, respecting spawn weights.
 *
 * @return The data class representing the enemy type to spawn, or nullptr if no suitable enemy type was found.
 */
void UCEnemySpawnerComponent::GetAsyncRandomSpawnableEnemyData(const FVector& SpawnLocation) const
{
	// Automatically reject enemy spawns if the enemy data table hasn't been set.
	if (!ensureAlwaysMsgf(EnemyTable, TEXT("Enemy data table must be set to be able to spawn enemies."))) return;

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
		if (RandomWeight <= 0)
		{
			const TArray<FName> Bundles;
			const FStreamableDelegate Delegate = FStreamableDelegate::CreateUObject(this, &UCEnemySpawnerComponent::SpawnEnemyAtLocation, EnemyRow->EnemyAssetId, SpawnLocation);
			UAssetManager::GetIfInitialized()->LoadPrimaryAsset(EnemyRow->EnemyAssetId, Bundles, Delegate);

			break;
		}
	}
}

// ReSharper disable twice CppPassValueParameterByConstReference ~ Incorrect suggestion
void UCEnemySpawnerComponent::SpawnEnemyAtLocation(FPrimaryAssetId LoadedAssetId, const FVector SpawnLocation) const
{
	UCEnemyData* EnemyData = Cast<UCEnemyData>(UAssetManager::GetIfInitialized()->GetPrimaryAssetObject(LoadedAssetId));
	if (!ensureMsgf(EnemyData, TEXT("Was unable to get the in-memory reference for an EnemyData that was loaded. Perhaps an issue with async loading or assets?"))) return;

	FActorSpawnParameters AICharacterSpawnParameters;
	AICharacterSpawnParameters.Owner = GetOwner();

	if (ACAICharacter* SpawnedAICharacter = Cast<ACAICharacter>(GetWorld()->SpawnActor<AActor>(EnemyData->EnemyClass, SpawnLocation, FRotator::ZeroRotator)))
	{
		SpawnedAICharacter->SetCoinRewardUponDeath(EnemyData->CoinRewardUponDeath);
		if (UCActionComponent* ActionComponent = UCActionComponent::GetComponentFrom(SpawnedAICharacter))
		{
			// TODO: Actions are currently unused as they'd require major BT and GAS refactors to make them work with each other. This is more for demonstration than anything else.
			for (const TSubclassOf<UCBaseAction> Action : EnemyData->EnemyActions) { ActionComponent->AddAction(Action, SpawnedAICharacter); }
		}
	}
}

// ReSharper disable once CppTooWideScopeInitStatement ~ Results in worse readability
uint16 UCEnemySpawnerComponent::GetNumberOfEnemiesAlive() const
{
	uint16 NumberOfEnemiesAlive = 0;
	for (TActorIterator<ACAICharacter> Iterator(GetWorld()); Iterator; ++Iterator)
	{
		const UCAttributeComponent* AttributeComponent = UCAttributeComponent::GetComponentFrom(*Iterator);
		if (AttributeComponent && AttributeComponent->IsAlive()) NumberOfEnemiesAlive++;
	}

	return NumberOfEnemiesAlive;
}

/**
 * @brief Checks if the game mode is able to spawn more enemies.
 *
 * @return True if the game mode can spawn more enemies, false otherwise.
 */
bool UCEnemySpawnerComponent::CanGameModeSpawnMoreEnemies(const uint16 NumberOfEnemiesAlive) const
{
	// Automatically reject enemy spawns if the difficulty curve hasn't been set.
	if (!ensureAlwaysMsgf(MaxEnemyCountOverTimeCurve, TEXT("Difficulty curve asset must be set for GameMode."))) return false;

	const uint16 MaxNumberOfEnemiesAlive = MaxEnemyCountOverTimeCurve->GetFloatValue(GetWorld()->TimeSeconds);
	return NumberOfEnemiesAlive < MaxNumberOfEnemiesAlive;
}
