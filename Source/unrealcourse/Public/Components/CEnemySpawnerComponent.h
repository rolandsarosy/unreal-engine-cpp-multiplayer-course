#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "CEnemySpawnerComponent.generated.h"

class UCEnemyData;
class UEnvQuery;
class ACAICharacter;

/* DataTable Row for describing enemy spawning weights and types. */
USTRUCT(BlueprintType)
struct FEnemyInfoRow : public FTableRowBase
{
	GENERATED_BODY()

	/* Default values */
	FEnemyInfoRow()
	{
		SpawnWeight = 1.0f;
	}

	/* DataAsset related to the unit. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UCEnemyData> EnemyData;
	
	/* Relative chance to spawn this unit. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SpawnWeight;
};

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
	TObjectPtr<UDataTable> EnemyTable;

	UPROPERTY(EditDefaultsOnly, Category="AI")
	TObjectPtr<UCurveFloat> MaxEnemyCountOverTimeCurve;

	UFUNCTION()
	void OnSpawnLocationQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	void OnSpawnEnemyTimerElapsed();

	void SpawnEnemyAtLocation(const FVector& SpawnLocation) const;

	uint16 GetNumberOfEnemiesAlive() const;

	TSubclassOf<ACAICharacter> GetEnemyTypeToSpawn() const;

	bool CanGameModeSpawnMoreEnemies(uint16 NumberOfEnemiesAlive) const;

	virtual void BeginPlay() override;
};
