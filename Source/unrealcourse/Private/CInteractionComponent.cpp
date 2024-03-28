#include "CInteractionComponent.h"

#include "CGameplayInterface.h"
#include "DrawDebugHelpers.h"

UCInteractionComponent::UCInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UCInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCInteractionComponent::PrimaryInteract()
{
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	FVector EyeLocation;
	FRotator EyeRotation;
	GetOwner()->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	FVector End = EyeLocation + (EyeRotation.Vector() * 1000);

	TArray<FHitResult> HitResults;
	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(HitResults, EyeLocation, End, FQuat::Identity, ObjectQueryParams, FCollisionShape::MakeSphere(30));

	for (FHitResult SingleHit : HitResults)
	{
		if (AActor* HitActor = SingleHit.GetActor(); HitActor && HitActor->Implements<UCGameplayInterface>())
		{
			APawn* Pawn = Cast<APawn>(GetOwner());
			ICGameplayInterface::Execute_Interact(HitActor, Pawn);
			
			DrawDebugSphere(GetWorld(), SingleHit.Location, 30.0F, 32, bBlockingHit ? FColor::Green : FColor::Red, false, 1.0F);
			break;
		}
	}

	DrawDebugLine(GetWorld(), EyeLocation, End, bBlockingHit ? FColor::Green : FColor::Red, false, 1.5, 0, 1.0F);
}
