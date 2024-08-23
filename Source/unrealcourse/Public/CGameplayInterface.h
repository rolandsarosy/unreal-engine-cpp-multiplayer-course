#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CGameplayInterface.generated.h"

UINTERFACE(MinimalAPI)
class UCGameplayInterface : public UInterface
{
	GENERATED_BODY()
};

class UNREALCOURSE_API ICGameplayInterface
{
	GENERATED_BODY()

public:
	/* Called after the Actor state was restored from a SaveGame file. Used in actors for restoring their states and replication. */
	UFUNCTION(BlueprintNativeEvent)
	void OnActorLoaded();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FText GetInteractText(APawn* InstigatorPawn);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Interact(APawn* InstigatorPawn);
};
