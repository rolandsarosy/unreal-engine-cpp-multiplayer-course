#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "CSaveGameSettings.generated.h"

UCLASS(Config=Game, DefaultConfig, meta=(DisplayName="Save Game Settings"))
class UNREALCOURSE_API UCSaveGameSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UCSaveGameSettings();

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="SaveGame")
	FString SaveSlotName;
};
