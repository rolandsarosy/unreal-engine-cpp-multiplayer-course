#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQuery.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "GameFramework/GameModeBase.h"
#include "CGameModeBase.generated.h"

class UEnvQuery;

UCLASS()
class UNREALCOURSE_API ACGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ACGameModeBase();

	virtual void StartPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly, Category="AI")
	float SpawnTimerInterval;

	UPROPERTY(EditDefaultsOnly, Category="AI")
	TObjectPtr<UEnvQuery> SpawnEnemyQuery;

	UPROPERTY(EditDefaultsOnly, Category="AI")
	TSubclassOf<AActor> EnemyClass;

	UPROPERTY(EditDefaultsOnly, Category="AI")
	TObjectPtr<UCurveFloat> DifficultyCurve;

private:
	UFUNCTION()
	void OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	void OnSpawnEnemyTimerElapsed();

	void SpawnEnemyAtLocation(const FVector& SpawnLocation) const;

	uint16 GetNumberOfEnemiesAlive() const;

	bool CanGameModeSpawnMoreEnemies(uint16 NumberOfEnemiesAlive) const;
};
