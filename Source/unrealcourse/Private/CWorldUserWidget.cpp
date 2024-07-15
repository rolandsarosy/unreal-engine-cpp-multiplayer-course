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
	FVector AttachedActorOrigin;
	FVector AttachedActorBoxExtent;
	AttachedActor->GetActorBounds(true, AttachedActorOrigin, AttachedActorBoxExtent);

	FVector ActorLocation = AttachedActor->GetActorLocation();

	switch (WorldUserWidgetOffsetType)
	{
	case Percentage:
		ActorLocation.Z += AttachedActorBoxExtent.Z * (1.0f + (static_cast<float>(VerticalOffsetPercentage) / 100.0f));
		break;
	case Value:
		ActorLocation.Z += AttachedActorBoxExtent.Z + VertialOffsetValue;
		break;
	default:
		ActorLocation.Z += AttachedActorBoxExtent.Z;
		break;
	}

	return ActorLocation;
}
