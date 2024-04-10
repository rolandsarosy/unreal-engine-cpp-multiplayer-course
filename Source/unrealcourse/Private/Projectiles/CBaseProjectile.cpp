#include "Projectiles/CBaseProjectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"

ACBaseProjectile::ACBaseProjectile()
{
	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SphereComponent->SetCollisionProfileName("Projectile");
	RootComponent = SphereComponent;

	ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>("ParticleSystemComponent");
	ParticleSystemComponent->SetupAttachment(RootComponent);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bInitialVelocityInLocalSpace = true;
}


void ACBaseProjectile::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle TimerHandle = FTimerHandle();
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ACBaseProjectile::AutoDestroyProjectile, 10.0f, false);
}

void ACBaseProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SphereComponent->IgnoreActorWhenMoving(GetInstigator(), true);
}

void ACBaseProjectile::AutoDestroyProjectile()
{
	Destroy();
}
