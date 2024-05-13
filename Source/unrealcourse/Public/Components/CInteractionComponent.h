#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CInteractionComponent.generated.h"

class UCWorldUserWidget;

UCLASS(ClassGroup=(Custom))
class UNREALCOURSE_API UCInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCInteractionComponent();

	void PrimaryInteract();

private:
	UPROPERTY()
	AActor* FocusedActor;

	UPROPERTY(EditDefaultsOnly, Category="UI")
	TSubclassOf<UCWorldUserWidget> DefaultWidgetClass;

	UPROPERTY(VisibleAnywhere, Category="UI")
	UCWorldUserWidget* DefaultWidgetInstance;

	UPROPERTY(EditDefaultsOnly, Category="Interaction Trace", meta=(Tooltip = "The trace's max distance from the Player's viewpont, in Unreal Units."))
	float TraceDistance;

	UPROPERTY(EditDefaultsOnly, Category="Interaction Trace", meta=(Tooltip = "The trace sweep sphere's radius, in Unreal Units."))
	float TraceRadius;

	UPROPERTY(EditDefaultsOnly, Category="Interaction Trace", meta=(Tooltip = "The trace's frequency in seconds. Decimals values are supported and encouraged."))
	float TraceFrequency;

	UPROPERTY(EditDefaultsOnly, Category="Interaction Trace", meta=(Tooltip = "The trace's collision channel to trace against."))
	TEnumAsByte<ECollisionChannel> CollisionChannel;

	virtual void BeginPlay() override;
	
	void FindBestInteractable();

	void SetWorldWidget();
};
