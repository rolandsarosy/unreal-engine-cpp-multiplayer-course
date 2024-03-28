#include "ExplosiveBarrel.h"

#include "PhysicsEngine/RadialForceComponent.h"

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
	RadialForceComponent->ImpulseStrength = 150000.0f;
}

void AExplosiveBarrel::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	StaticMeshComponent->OnComponentHit.AddDynamic(this, &AExplosiveBarrel::OnComponentHit);
}

void AExplosiveBarrel::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0F, FColor::Red, TEXT("Barrel explosion triggered!"));

	RadialForceComponent->FireImpulse();
}
