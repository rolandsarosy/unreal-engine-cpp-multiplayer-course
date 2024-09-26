#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CEnemyData.generated.h"

class ACAICharacter;
class UCBaseAction;

UCLASS()
class UNREALCOURSE_API UCEnemyData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Spawn Info")
	TSubclassOf<ACAICharacter> EnemyClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Spawn Info")
	TArray<TSubclassOf<UCBaseAction>> EnemyActions;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Spawn Info")
	int CoinRewardUponDeath;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Spawn Info")
	TObjectPtr<UTexture2D> Icon;
};
