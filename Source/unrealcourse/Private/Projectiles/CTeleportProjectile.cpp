#include "Projectiles/CTeleportProjectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"

ACTeleportProjectile::ACTeleportProjectile()
{
	ProjectileMovementComponent->InitialSpeed = 4000;
	ProjectileMovementComponent->MaxSpeed = 0;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;

	TeleportExplosionEffectComponent = CreateDefaultSubobject<UParticleSystemComponent>("TeleportExplosionParticleSystem");
	TeleportExplosionEffectComponent->SetupAttachment(RootComponent);
	TeleportExplosionEffectComponent->SetAutoActivate(false);
}

void ACTeleportProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SphereComponent->OnComponentHit.AddDynamic(this, &ACTeleportProjectile::OnComponentHit);
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ACTeleportProjectile::OnComponentBeginOverlap);
}

void ACTeleportProjectile::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(TimerHandle_TeleportEffect, this, &ACTeleportProjectile::OnStartTeleportEffect, 0.2F);
}

void ACTeleportProjectile::OnStartTeleportEffect()
{
	TeleportExplosionEffectComponent->ActivateSystem();
	ProjectileMovementComponent->StopMovementImmediately();
	SetActorEnableCollision(false);
	GetWorldTimerManager().SetTimer(TimerHandle_TeleportAction, this, &ACTeleportProjectile::OnTeleport, 0.2F);
}

void ACTeleportProjectile::OnTeleport()
{
	GetInstigator()->TeleportTo(GetActorLocation(), GetActorRotation(), false, true);
	Destroy();
}

void ACTeleportProjectile::OnProjectileExplosion(const AActor* HitActor)
{
	if (HitActor != GetInstigator())
	{
		GetWorldTimerManager().ClearAllTimersForObject(this);

		TimerHandle_TeleportEffect.Invalidate();
		TimerHandle_TeleportAction.Invalidate();
		OnStartTeleportEffect();
	}
}

void ACTeleportProjectile::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Separating Hit and Overlap as there might be different behavior to Hit down the line.
	OnProjectileExplosion(OtherActor);
}

void ACTeleportProjectile::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                                   const FHitResult& SweepResult)
{
	// Separating Hit and Overlap as there might be different behavior to Hit down the line.
	OnProjectileExplosion(OtherActor);
}
