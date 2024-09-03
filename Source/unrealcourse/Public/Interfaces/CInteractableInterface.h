#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CInteractableInterface.generated.h"

UINTERFACE(MinimalAPI)
class UCInteractableInterface : public UInterface
{
	GENERATED_BODY()
};

class UNREALCOURSE_API ICInteractableInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FText GetInteractText(APawn* InstigatorPawn);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Interact(APawn* InstigatorPawn);
};
