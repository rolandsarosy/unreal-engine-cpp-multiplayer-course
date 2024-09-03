#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CSaveableInterface.generated.h"

UINTERFACE(MinimalAPI)
class UCSaveableInterface : public UInterface
{
	GENERATED_BODY()
};

class UNREALCOURSE_API ICSaveableInterface
{
	GENERATED_BODY()

public:
	/* Called after the Actor state was restored from a SaveGame file. Used in actors for restoring their states and replication. */
	UFUNCTION(BlueprintNativeEvent)
	void OnActorLoaded();
};
