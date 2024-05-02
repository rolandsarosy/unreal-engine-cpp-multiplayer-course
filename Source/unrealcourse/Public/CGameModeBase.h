#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQuery.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "GameFramework/GameModeBase.h"
#include "CGameModeBase.generated.h"

class UCPickupSpawnerComponent;
class ACAICharacter;
class UEnvQuery;

UCLASS()
class UNREALCOURSE_API ACGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ACGameModeBase();

	virtual void StartPlay() override;

	virtual void OnActorKilled(AActor* Victim, AActor* Killer);

protected:
	UPROPERTY(EditDefaultsOnly, Category="AI")
	float EnemySpawnTimerInterval;

	UPROPERTY(EditDefaultsOnly, Category="Components") 
	TObjectPtr<UCPickupSpawnerComponent> PickupSpawnerComponent;

	UPROPERTY(EditDefaultsOnly, Category="AI")
	TObjectPtr<UEnvQuery> SpawnEnemyEQ;

	UPROPERTY(EditDefaultsOnly, Category="AI")
	TSubclassOf<ACAICharacter> SpawnEnemyClass;

	UPROPERTY(EditDefaultsOnly, Category="AI")
	TObjectPtr<UCurveFloat> MaxEnemyCountOverTimeCurve;

	UPROPERTY(EditDefaultsOnly, Category="GameMode")
	float PlayerRespawnDelay;

	UFUNCTION()
	void RespawnPlayer(AController* PlayerController);

private:
	UFUNCTION()
	void OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	void OnSpawnEnemyTimerElapsed();

	void SpawnEnemyAtLocation(const FVector& SpawnLocation) const;

	uint16 GetNumberOfEnemiesAlive() const;

	bool CanGameModeSpawnMoreEnemies(uint16 NumberOfEnemiesAlive) const;
};
