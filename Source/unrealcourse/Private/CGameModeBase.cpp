#include "CGameModeBase.h"

#include "EngineUtils.h"
#include "AI/CAICharacter.h"
#include "Components/CAttributeComponent.h"
#include "EnvironmentQuery/EnvQueryManager.h"

ACGameModeBase::ACGameModeBase()
{
	SpawnTimerInterval = 2.0f;
}

void ACGameModeBase::StartPlay()
{
	Super::StartPlay();

	// Continuous timer to spawn in more enemies. Actual amount of enemies and whether it is allowed to spawn determined by spawn logic down in the chain.
	FTimerHandle TimerHandle_SpawnEnemies;
	GetWorldTimerManager().SetTimer(TimerHandle_SpawnEnemies, this, &ACGameModeBase::OnSpawnEnemyTimerElapsed, SpawnTimerInterval, true);
}

void ACGameModeBase::OnSpawnEnemyTimerElapsed()
{
	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(GetWorld(), SpawnEnemyQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);

	if (!ensure(QueryInstance)) return;
	QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ACGameModeBase::OnQueryCompleted);
}

// ReSharper disable once CppMemberFunctionMayBeConst ~ Incorrect suggestion
// ReSharper disable once CppParameterMayBeConstPtrOrRef ~ Incorrect suggestion
void ACGameModeBase::OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, const EEnvQueryStatus::Type QueryStatus)
{
	switch (QueryStatus)
	{
	case EEnvQueryStatus::Success:
		if (CanGameModeSpawnMoreEnemies(GetNumberOfEnemiesAlive()) && QueryInstance->GetResultsAsLocations().IsValidIndex(0))
		{
			SpawnEnemyAtLocation(QueryInstance->GetResultsAsLocations()[0]);
		}
	case EEnvQueryStatus::Processing:
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("Enemy spawn EQS did not succeed."));
	}
}

void ACGameModeBase::SpawnEnemyAtLocation(const FVector& SpawnLocation) const
{
	const AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(EnemyClass, SpawnLocation, FRotator::ZeroRotator);
	UE_LOG(LogTemp, Log, TEXT("%s attempted to spawn enemy class of %s at %s. Return value was: %p"),
	       *ACGameModeBase::StaticClass()->GetName(), *EnemyClass->StaticClass()->GetName(), *SpawnLocation.ToString(), SpawnedActor);
}

bool ACGameModeBase::CanGameModeSpawnMoreEnemies(const uint16 NumberOfEnemiesAlive) const
{
	if (!ensureMsgf(DifficultyCurve, TEXT("Difficulty curve asset must be set for GameMode."))) return false; // Automatically reject enemy spawns if the difficulty curve hasn't been set yet.

	const uint16 MaxNumberOfEnemiesAlive = DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);
	return NumberOfEnemiesAlive < MaxNumberOfEnemiesAlive;
}

// ReSharper disable once CppTooWideScopeInitStatement ~ Results in worse readability
uint16 ACGameModeBase::GetNumberOfEnemiesAlive() const
{
	uint16 NumberOfEnemiesAlive = 0;
	for (TActorIterator<ACAICharacter> Iterator(GetWorld()); Iterator; ++Iterator)
	{
		const ACAICharacter* Enemy = *Iterator;

		const UCAttributeComponent* AttributeComponent = Cast<UCAttributeComponent>(Enemy->GetComponentByClass(UCAttributeComponent::StaticClass()));
		if (AttributeComponent && AttributeComponent->IsAlive()) NumberOfEnemiesAlive++;
	}

	return NumberOfEnemiesAlive;
}
