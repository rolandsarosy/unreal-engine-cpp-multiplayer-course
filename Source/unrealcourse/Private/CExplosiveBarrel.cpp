#include "CExplosiveBarrel.h"

#include "PhysicsEngine/RadialForceComponent.h"
#include "DrawDebugHelpers.h"

AExplosiveBarrel::AExplosiveBarrel()
{
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	StaticMeshComponent->SetSimulatePhysics(true);
	StaticMeshComponent->SetCollisionProfileName("PhysicsActor");

	RootComponent = StaticMeshComponent;

	RadialForceComponent = CreateDefaultSubobject<URadialForceComponent>("RadialForceComponent");
	RadialForceComponent->SetupAttachment(StaticMeshComponent);
	RadialForceComponent->AddRelativeLocation(FVector(0, 0, RadialForceComponent->GetComponentLocation().Z + 80));
	RadialForceComponent->Radius = 400.0f;
	RadialForceComponent->ImpulseStrength = 100000.0f;
}

void AExplosiveBarrel::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	StaticMeshComponent->OnComponentHit.AddDynamic(this, &AExplosiveBarrel::OnComponentHit);
	StaticMeshComponent->OnComponentBeginOverlap.AddDynamic(this, &AExplosiveBarrel::OnComponentOverlap);
}

// ReSharper disable once CppMemberFunctionMayBeConst ~ Incorrect suggestion
void AExplosiveBarrel::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	RadialForceComponent->FireImpulse();
}

// ReSharper disable once CppMemberFunctionMayBeConst ~ Incorrect suggestion
void AExplosiveBarrel::OnComponentOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                          const FHitResult& SweepResult)
{
	RadialForceComponent->FireImpulse();
}
