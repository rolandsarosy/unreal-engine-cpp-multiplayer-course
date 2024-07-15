#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "CEnemySpawnerComponent.generated.h"

class UEnvQuery;
class ACAICharacter;

UCLASS(ClassGroup=(Custom))
class UNREALCOURSE_API UCEnemySpawnerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCEnemySpawnerComponent();

private:
	UPROPERTY(EditDefaultsOnly, Category="AI")
	float EnemySpawnTimerInterval;

	UPROPERTY(EditDefaultsOnly, Category="AI")
	TObjectPtr<UEnvQuery> SpawnEnemyEQ;

	UPROPERTY(EditDefaultsOnly, Category="AI")
	TSubclassOf<ACAICharacter> SpawnEnemyClass;

	UPROPERTY(EditDefaultsOnly, Category="AI")
	TObjectPtr<UCurveFloat> MaxEnemyCountOverTimeCurve;

	UFUNCTION()
	void OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);
	
	void OnSpawnEnemyTimerElapsed();
	
	void SpawnEnemyAtLocation(const FVector& SpawnLocation) const;

	uint16 GetNumberOfEnemiesAlive() const;

	bool CanGameModeSpawnMoreEnemies(uint16 NumberOfEnemiesAlive) const;

	virtual void BeginPlay() override;
};
