#include "Projectiles/CMagicProjectile.h"

#include "Components/CAttributeComponent.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

ACMagicProjectile::ACMagicProjectile()
{
	ProjectileMovementComponent->InitialSpeed = 2000;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;

	LoopingAudioComponent = CreateDefaultSubobject<UAudioComponent>("LoopingAudioComponent");
	LoopingAudioComponent->SetupAttachment(RootComponent);

	DamageAmount = 20.0f;
}

void ACMagicProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SphereComponent->OnComponentHit.AddDynamic(this, &ACMagicProjectile::OnComponentHit);
}

void ACMagicProjectile::BeginPlay()
{
	Super::BeginPlay();
}

// ReSharper disable once CppParameterMayBeConstPtrOrRef ~ Incorrect suggestion
void ACMagicProjectile::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && OtherActor != GetInstigator())
	{
		// If the target has an AttributeComponent attached, apply damage.
		if (UCAttributeComponent* AttributeComponent = Cast<UCAttributeComponent>(OtherActor->GetComponentByClass(UCAttributeComponent::StaticClass())))
		{
			AttributeComponent->ApplyHealthChange(GetInstigator(), -DamageAmount);
		}

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticleSystem, GetActorLocation(), GetActorRotation());
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), Cast<USoundBase>(ImpactSoundCue), GetActorLocation(), GetActorRotation());
		UGameplayStatics::PlayWorldCameraShake(GetWorld(), ImpactCameraShake, GetActorLocation(), 15, 1200, 0.7f, true);
		Destroy();
	}
}
