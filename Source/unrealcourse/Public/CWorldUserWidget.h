#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CWorldUserWidget.generated.h"

class USizeBox;

UCLASS(Abstract)
class UNREALCOURSE_API UCWorldUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category="UI")
	TObjectPtr<AActor> AttachedActor;

protected:
	UPROPERTY(meta=(BindWidget)) // BindWidget will make sure we cannot compile the child until the exact class has been set with the exact variable  name.
	TObjectPtr<USizeBox> ParentSizeBox;

	UPROPERTY(EditDefaultsOnly, meta=(Tooltip = "Marks how many % will the center of this widget be offset from the top of the BoundingBox of the attached Actor. Can be negative."))
	int8 VerticalOffsetPercentage;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	FVector CalculateOffsetScreenPosition() const;
};
