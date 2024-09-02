#include "Components/CInteractionComponent.h"

#include "CGameplayInterface.h"
#include "DrawDebugHelpers.h"
#include "Blueprint/UserWidget.h"
#include "CWorldUserWidget.h"

static TAutoConsoleVariable CVarDebugDrawInteraction(TEXT("course.DrawDebugInteraction"), false, TEXT("Flags whether the interaction component should draw debug helpers in world."), ECVF_Cheat);

UCInteractionComponent::UCInteractionComponent()
{
	FocusedActor = nullptr;
	DefaultWidgetInstance = nullptr;

	TraceDistance = 500;
	TraceRadius = 30;
	TraceFrequency = 0.15;

	CollisionChannel = ECC_WorldDynamic;
}

void UCInteractionComponent::PrimaryInteract() { ServerInteract(FocusedActor); }

void UCInteractionComponent::ServerInteract_Implementation(AActor* InFocus) { if (InFocus) ICGameplayInterface::Execute_Interact(InFocus, Cast<APawn>(GetOwner())); }

void UCInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// I'm really strongly againts using timers to resolve lifecyle function-related issues but this one is particularly nasty.
	// @note https://forums.unrealengine.com/t/is-locally-controlled-returns-false-for-listen-server-host/406471/2
	// TLDR.: For the first few ticks, the ListenServer does not know that it has sole ownership of the pawn (IsLocallyControlled) returns false.
	// I've tried various fixes, spent several hours here to try and hook onto a lifecycle event anywhere that's not this nasty, but I just did not find a solution that
	// was an improvement and also worked for both ListenServers and Clients. This way, I'm only hacking the host and not the clients at the very least.
	//
	// If you are reading this and have a better idea, please reach out to me here or at roland.sarosy@gmail.com.
	if (GetWorld()->GetNetMode() == NM_ListenServer || GetWorld()->GetNetMode() == NM_DedicatedServer)
	{
		FTimerHandle DelayTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(DelayTimerHandle, this, &UCInteractionComponent::StartTraceTimerConditionally, 0.3f, false);
	}
	else
	{
		StartTraceTimerConditionally();
	}
}

void UCInteractionComponent::StartTraceTimerConditionally()
{
	if (Cast<APawn>(GetOwner())->IsLocallyControlled())
	{
		DefaultWidgetInstance = CreateWidget<UCWorldUserWidget>(GetWorld(), DefaultWidgetClass);
		GetWorld()->GetTimerManager().SetTimer(FrequencyTimerHandle, [this] { FindBestInteractable(); }, TraceFrequency, true);
	}
}

/**
 * Finds the best interactable actor based on the current player's view and performs necessary actions such as setting the focused actor and showing the relevant world UI.
 *
 * @note It was required during class to do a timer-based, player-owned solution to the interaction problem instead of having the actors in the world,
 * report back to the player that they're interactable.
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
	GetWorld()->GetTimerManager().ClearTimer(FrequencyTimerHandle);
	if (DefaultWidgetInstance && DefaultWidgetInstance->IsInViewport()) DefaultWidgetInstance->RemoveFromParent();

	Super::EndPlay(EndPlayReason);
}
