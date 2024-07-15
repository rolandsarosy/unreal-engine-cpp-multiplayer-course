#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CWorldUserWidget.generated.h"

class USizeBox;

UENUM(BlueprintType)
enum EWorldUserWidgetOffsetType : uint8
{
	None = 0 UMETA(DisplayName = "None"),
	Percentage = 1 UMETA(DisplayName = "Percentage"),
	Value = 2 UMETA(DisplayName = "Value")
};

UCLASS(Abstract)
class UNREALCOURSE_API UCWorldUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category="UI", meta=(ExposeOnSpawn = true))
	TObjectPtr<AActor> AttachedActor;

protected:
	UPROPERTY(meta=(BindWidget)) // BindWidget will make sure we cannot compile the child until the exact class has been set with the exact variable  name.
	TObjectPtr<USizeBox> ParentSizeBox;

	UPROPERTY(EditDefaultsOnly, meta=(Tooltip = "Defines which type of offset will be used."))
	TEnumAsByte<EWorldUserWidgetOffsetType> WorldUserWidgetOffsetType;

	UPROPERTY(EditDefaultsOnly,
		meta=(EditCondition = "WorldUserWidgetOffsetType == 1", Tooltip =
			"Marks how many % will the center of this widget be offset from the top of the BoundingBox of the attached Actor. Can be negative."))
	int8 VerticalOffsetPercentage;

	UPROPERTY(EditDefaultsOnly,
		meta=(EditCondition = "WorldUserWidgetOffsetType == 2", Tooltip =
			"Marks how many pixels will the center of this widget be offset from the top of the BoundingBox of the attached Actor. Can be negative."))
	int32 VertialOffsetValue;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	FVector CalculateOffsetScreenPosition() const;
};
