#include "CGameModeBase.h"

#include "CCharacter.h"
#include "CPlayerState.h"
#include "EngineUtils.h"
#include "AI/CAICharacter.h"
#include "Components/CAttributeComponent.h"
#include "Components/CPickupSpawnerComponent.h"
#include "EnvironmentQuery/EnvQueryManager.h"

static TAutoConsoleVariable CVarSpawnEnemies(TEXT("course.SpawnEnemies"), true, TEXT("Enabled or disables spawning of enemies via timers."), ECVF_Cheat);

ACGameModeBase::ACGameModeBase()
{
	PickupSpawnerComponent = CreateDefaultSubobject<UCPickupSpawnerComponent>("PickupSpawnerComponent");
	
	EnemySpawnTimerInterval = 2.0f;
	PlayerRespawnDelay = 5.0f;
}

void ACGameModeBase::StartPlay()
{
	Super::StartPlay();
	PickupSpawnerComponent->SpawnItems();

	// Continuous timer to spawn in more enemies. Actual amount of enemies and whether it is allowed to spawn determined by spawn logic down in the chain.
	FTimerHandle TimerHandle_SpawnEnemies;
	GetWorldTimerManager().SetTimer(TimerHandle_SpawnEnemies, this, &ACGameModeBase::OnSpawnEnemyTimerElapsed, EnemySpawnTimerInterval, true);

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
		UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(GetWorld(), SpawnEnemyEQ, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);

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
	const AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(SpawnEnemyClass, SpawnLocation, FRotator::ZeroRotator);

#if WITH_EDITOR
	UE_LOG(LogTemp, Log, TEXT("%s attempted to spawn enemy class of %s at %s. Return value was: %p"),
	       *GetNameSafe(this), *GetNameSafe(SpawnEnemyClass), *SpawnLocation.ToString(), *GetNameSafe(SpawnedActor));

	if (SpawnedActor)
	{
		DrawDebugSphere(GetWorld(), SpawnLocation, 15.0f, 16, FColor::Yellow, false, 5.0f, 0, 1);
		DrawDebugString(GetWorld(), SpawnLocation, TEXT("Enemy Spawned Here"), nullptr, FColor::Yellow, 5.0f, true);
	}
#endif
}

bool ACGameModeBase::CanGameModeSpawnMoreEnemies(const uint16 NumberOfEnemiesAlive) const
{
	// Automatically reject enemy spawns if the difficulty curve hasn't been set yet.
	if (!ensureMsgf(MaxEnemyCountOverTimeCurve, TEXT("Difficulty curve asset must be set for GameMode."))) return false;

	const uint16 MaxNumberOfEnemiesAlive = MaxEnemyCountOverTimeCurve->GetFloatValue(GetWorld()->TimeSeconds);
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
	if (Victim->IsA(SpawnEnemyClass) && Killer->IsA(ACCharacter::StaticClass()))
	{
		Cast<ACPlayerState>(Cast<APawn>(Killer)->GetPlayerState())->AddCoins(Victim, Cast<ACAICharacter>(Victim)->CoinRewardUponDeath);
		return;
	}
}

void ACGameModeBase::RespawnPlayer(AController* PlayerController)
{
	if (!ensureMsgf(PlayerController, TEXT("Was unable to respawn player as the Controller was invalid."))) return;

	PlayerController->UnPossess();
	RestartPlayer(PlayerController);
}
