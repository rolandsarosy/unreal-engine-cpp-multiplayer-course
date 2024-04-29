#include "CGameModeBase.h"

#include "CCharacter.h"
#include "EngineUtils.h"
#include "AI/CAICharacter.h"
#include "Components/CAttributeComponent.h"
#include "EnvironmentQuery/EnvQueryManager.h"

static TAutoConsoleVariable CVarSpawnEnemies(TEXT("course.SpawnEnemies"), true, TEXT("Enabled or disables spawning of enemies via timers."), ECVF_Cheat);

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
	if (!CVarSpawnEnemies.GetValueOnGameThread())
	{
		UE_LOG(LogTemp, Warning, TEXT("Enemy Spawn"))
		return;
	}

	if (CanGameModeSpawnMoreEnemies(GetNumberOfEnemiesAlive()))
	{
		UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(GetWorld(), SpawnEnemyQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);

		if (!ensure(QueryInstance)) return;
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ACGameModeBase::OnQueryCompleted);
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst ~ Incorrect suggestion
// ReSharper disable once CppParameterMayBeConstPtrOrRef ~ Incorrect suggestion
void ACGameModeBase::OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, const EEnvQueryStatus::Type QueryStatus)
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

void ACGameModeBase::SpawnEnemyAtLocation(const FVector& SpawnLocation) const
{
	const AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(EnemyClass, SpawnLocation, FRotator::ZeroRotator);
	UE_LOG(LogTemp, Log, TEXT("%s attempted to spawn enemy class of %s at %s. Return value was: %p"), *this->GetName(), *EnemyClass->GetName(), *SpawnLocation.ToString(), SpawnedActor);

#if WITH_EDITOR
	if (SpawnedActor)
	{
		DrawDebugSphere(GetWorld(), SpawnLocation, 15.0f, 16, FColor::Yellow, false, 5.0f, 0, 1);
		DrawDebugString(GetWorld(), SpawnLocation, TEXT("Enemy Spawned Here"), nullptr, FColor::Yellow, 5.0f, true);
	}
#endif
}

bool ACGameModeBase::CanGameModeSpawnMoreEnemies(const uint16 NumberOfEnemiesAlive) const
{
	if (!ensureMsgf(DifficultyCurve, TEXT("Difficulty curve asset must be set for GameMode."))) return false; // Automatically reject enemy spawns if the difficulty curve hasn't been set yet.

	const uint16 MaxNumberOfEnemiesAlive = DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);
	return NumberOfEnemiesAlive < MaxNumberOfEnemiesAlive;
}

// TODO - This is a computationally expensive call. It was written during class, but I'd like to write a more efficient solution later on.
// ReSharper disable once CppTooWideScopeInitStatement ~ Results in worse readability
uint16 ACGameModeBase::GetNumberOfEnemiesAlive() const
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

// This is a cheat that'll work only in non-shipped builds. Conditional compiling is unnecessary as the console is disabled in shipped builds.
// ReSharper disable once CppTooWideScopeInitStatement ~ Results in worse readability
void ACGameModeBase::KillAllEnemies()
{
	for (TActorIterator<ACAICharacter> Iterator(GetWorld()); Iterator; ++Iterator)
	{
		ACAICharacter* Enemy = *Iterator;

		UCAttributeComponent* AttributeComponent = UCAttributeComponent::GetComponentFrom(Enemy);
		if (AttributeComponent && AttributeComponent->IsAlive()) AttributeComponent->Kill(this); // TODO: Consider passing the player instead for kill credit.
	}
}

void ACGameModeBase::OnActorKilled(AActor* Victim, AActor* Killer)
{
	if (ACCharacter* PlayerCharacter = Cast<ACCharacter>(Victim))
	{
		FTimerHandle TimerHandle_RespawnDelay;

		GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay, [this, PlayerCharacter] { RespawnPlayer(PlayerCharacter->GetController()); }, 2.0f, false);
	}

	// TODO: During Assignment 5, unify log calls to use the GetNameSafe way of calling the names, instead of other solutions.
	UE_LOG(LogTemp, Log, TEXT("OnActorKilled: Victim: %s, Killer: %s"), *GetNameSafe(Victim), *GetNameSafe(Killer));
}

void ACGameModeBase::RespawnPlayer(AController* PlayerController)
{
	if (!ensureMsgf(PlayerController, TEXT("Was unable to respawn player as the Controller was invalid."))) return;

	PlayerController->UnPossess();
	RestartPlayer(PlayerController);
}
