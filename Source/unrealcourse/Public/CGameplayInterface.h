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
	/* Called after the Actor state was restored from a SaveGame file. */
	UFUNCTION(BlueprintNativeEvent)
	void OnActorLoaded();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Interact(APawn* InstigatorPawn);
};
