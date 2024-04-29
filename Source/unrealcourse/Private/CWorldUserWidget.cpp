#include "CWorldUserWidget.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/SizeBox.h"

void UCWorldUserWidget::NativeTick(const FGeometry& MyGeometry, const float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!IsValid(AttachedActor))
	{
		RemoveFromParent();

		UE_LOG(LogTemp, Warning, TEXT("AttachedActor no longer valid, removing widget."))
		return;
	}

	FVector2D ScreenPositionResult;
	if (UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(GetOwningPlayer(), CalculateOffsetScreenPosition(), ScreenPositionResult, false))
	{
		if (ParentSizeBox) ParentSizeBox->SetRenderTranslation(ScreenPositionResult);
	}
}

FVector UCWorldUserWidget::CalculateOffsetScreenPosition() const
{
	const float VerticalOffsetMultiplier = 1.0f + (static_cast<float>(VerticalOffsetPercentage) / 100.0f);

	FVector AttachedActorOrigin;
	FVector AttachedActorBoxExtent;
	AttachedActor->GetActorBounds(false, AttachedActorOrigin, AttachedActorBoxExtent);

	FVector ActorLocation = AttachedActor->GetActorLocation();
	ActorLocation.Z += AttachedActorBoxExtent.Z * VerticalOffsetMultiplier;

	return ActorLocation;
}
