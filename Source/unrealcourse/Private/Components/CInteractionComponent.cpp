#include "Components/CInteractionComponent.h"

#include "CGameplayInterface.h"
#include "DrawDebugHelpers.h"

static TAutoConsoleVariable CVarDebugDrawInteraction(TEXT("course.DebugDrawInteraction"), false, TEXT("Flags whether the interaction component should draw debug helpers in world."), ECVF_Cheat);

void UCInteractionComponent::PrimaryInteract()
{
	bool DrawDebugLines = CVarDebugDrawInteraction.GetValueOnGameThread();

	TArray<FHitResult> HitResults;

	FVector EyeLocation;
	FRotator EyeRotation;
	GetOwner()->GetActorEyesViewPoint(EyeLocation, EyeRotation);
	FVector End = EyeLocation + (EyeRotation.Vector() * 1000);

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(HitResults, EyeLocation, End, FQuat::Identity, ObjectQueryParams, FCollisionShape::MakeSphere(30));

	for (FHitResult SingleHit : HitResults)
	{
		if (AActor* HitActor = SingleHit.GetActor(); HitActor && HitActor->Implements<UCGameplayInterface>())
		{
			ICGameplayInterface::Execute_Interact(HitActor, Cast<APawn>(GetOwner()));

			if (CVarDebugDrawInteraction.GetValueOnGameThread()) DrawDebugSphere(GetWorld(), SingleHit.Location, 30.0F, 32, bBlockingHit ? FColor::Green : FColor::Red, false, 1.0F);
			break;
		}
	}

	if (CVarDebugDrawInteraction.GetValueOnGameThread()) DrawDebugLine(GetWorld(), EyeLocation, End, bBlockingHit ? FColor::Green : FColor::Red, false, 1.5, 0, 1.0F);
}
