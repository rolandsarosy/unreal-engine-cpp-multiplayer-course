#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CGameModeBase.generated.h"

class UCEnemySpawnerComponent;
class UCPickupSpawnerComponent;

UCLASS()
class UNREALCOURSE_API ACGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ACGameModeBase();

	virtual void OnActorKilled(AActor* Victim, AActor* Killer);

private:
	UPROPERTY(EditDefaultsOnly, Category="Components")
	TObjectPtr<UCPickupSpawnerComponent> PickupSpawnerComponent;

	UPROPERTY(EditDefaultsOnly, Category="Components")
	TObjectPtr<UCEnemySpawnerComponent> EnemySpawnerComponent;

	UPROPERTY(EditDefaultsOnly, Category="GameMode")
	float PlayerRespawnDelay;

	UFUNCTION()
	void RespawnPlayer(AController* PlayerController);
};
