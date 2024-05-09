#include "Projectiles/CMagicProjectile.h"

#include "CGameplayFunctionLibrary.h"
#include "AbilitySystem/CActionComponent.h"
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
	MaxParryAmount = 1;
	CurrentParryAmount = 0;
}

void ACMagicProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ACMagicProjectile::OnComponentOverlap);
	SphereComponent->OnComponentHit.AddDynamic(this, &ACMagicProjectile::OnComponentHit);
}

void ACMagicProjectile::BeginPlay()
{
	Super::BeginPlay();
}

// ReSharper disable once CppTooWideScopeInitStatement ~ Results in worse readability
void ACMagicProjectile::OnComponentOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                           const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != GetInstigator())
	{
		const UCActionComponent* ActionComponent = Cast<UCActionComponent>(OtherActor->GetComponentByClass(UCActionComponent::StaticClass()));
		if (ActionComponent && ActionComponent->ActiveGameplayTags.HasTag(ParryTag) && CurrentParryAmount < MaxParryAmount)
		{
			CurrentParryAmount++;
			ProjectileMovementComponent->Velocity = -ProjectileMovementComponent->Velocity;
			SetInstigator(Cast<APawn>(OtherActor));
			return;
		}
		
		UCGameplayFunctionLibrary::ApplyDirectionalImpulseDamage(GetInstigator(), OtherActor, DamageAmount, SweepResult);
		PlayEffectsAndDestroy();
	}
}

// ReSharper disable once CppParameterMayBeConstPtrOrRef ~ Incorrect suggestion
void ACMagicProjectile::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Separating Hit and Overlap as there might be different behavior to Hit down the line.
	if (OtherActor && OtherActor != GetInstigator()) PlayEffectsAndDestroy();
}

void ACMagicProjectile::PlayEffectsAndDestroy()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticleSystem, GetActorLocation(), GetActorRotation());
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), Cast<USoundBase>(ImpactSoundCue), GetActorLocation(), GetActorRotation());
	UGameplayStatics::PlayWorldCameraShake(GetWorld(), ImpactCameraShake, GetActorLocation(), 15, 1200, 0.7f, true);
	Destroy();
}
