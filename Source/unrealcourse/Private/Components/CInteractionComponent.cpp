#include "Components/CInteractionComponent.h"

#include "CGameplayInterface.h"
#include "DrawDebugHelpers.h"
#include "Blueprint/UserWidget.h"
#include "CWorldUserWidget.h"

static TAutoConsoleVariable CVarDebugDrawInteraction(TEXT("course.DebugDrawInteraction"), false, TEXT("Flags whether the interaction component should draw debug helpers in world."), ECVF_Cheat);

UCInteractionComponent::UCInteractionComponent()
{
	FocusedActor = nullptr;
	DefaultWidgetInstance = nullptr;

	TraceDistance = 500;
	TraceRadius = 30;
	TraceFrequency = 0.15;

	CollisionChannel = ECC_WorldDynamic;
}

void UCInteractionComponent::PrimaryInteract()
{
	ServerInteract(FocusedActor);
}

void UCInteractionComponent::ServerInteract_Implementation(AActor* InFocus)
{
	if (!InFocus)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "Interaction Component: No focused actor to interact with.");
		return;
	}
	ICGameplayInterface::Execute_Interact(InFocus, Cast<APawn>(GetOwner()));
}

void UCInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	if (Cast<APawn>(GetOwner())->IsLocallyControlled())
	{
		DefaultWidgetInstance = CreateWidget<UCWorldUserWidget>(GetWorld(), DefaultWidgetClass);
		GetWorld()->GetTimerManager().SetTimer(FrequencyTimerHandle, [this] { FindBestInteractable(); }, TraceFrequency, true);
	}
}

/**
 * Finds the best interactable actor based on the current player's view and performs necessary actions such as setting the focused actor and showing the relevant world UI.
 *
 * @note It was required during class to do a timer-based, player-owned solution to the interaction problem instead of having the actors in the world, report back to the player
 * that they're interactable.
 */
void UCInteractionComponent::FindBestInteractable()
{
	FVector EyeLocation;
	FRotator EyeRotation;
	GetOwner()->GetActorEyesViewPoint(EyeLocation, EyeRotation);
	FVector LineEnd = EyeLocation + EyeRotation.Vector() * TraceDistance;

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(CollisionChannel);

	TArray<FHitResult> HitResults;
	bool bIsBlockingHit = GetWorld()->SweepMultiByObjectType(HitResults, EyeLocation, LineEnd, FQuat::Identity, ObjectQueryParams, FCollisionShape::MakeSphere(TraceRadius));

	FocusedActor = nullptr;

	for (FHitResult SingleHit : HitResults)
	{
		if (AActor* HitActor = SingleHit.GetActor(); HitActor && HitActor->Implements<UCGameplayInterface>())
		{
			FocusedActor = HitActor;
			if (CVarDebugDrawInteraction.GetValueOnGameThread()) DrawDebugSphere(GetWorld(), SingleHit.Location, TraceRadius, 32, bIsBlockingHit ? FColor::Green : FColor::Red, false, 0.0F);
			break;
		}
	}

	if (CVarDebugDrawInteraction.GetValueOnGameThread()) DrawDebugLine(GetWorld(), EyeLocation, LineEnd, bIsBlockingHit ? FColor::Green : FColor::Red, false, 1.5, 0, 0.0F);

	SetWorldWidget();
}

/**
 * Sets the single-instanced widget in the world based on the currently focused actor.
 *
 * @note The instance of the widget is created during BeginPlay. 
 */
void UCInteractionComponent::SetWorldWidget()
{
	if (FocusedActor)
	{
		DefaultWidgetInstance->AttachedActor = FocusedActor;
		if (!DefaultWidgetInstance->IsInViewport()) DefaultWidgetInstance->AddToViewport();
	}
	else
	{
		if (DefaultWidgetInstance) DefaultWidgetInstance->RemoveFromParent();
	}
}

void UCInteractionComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetWorld()->GetTimerManager().ClearTimer(FrequencyTimerHandle);
}
