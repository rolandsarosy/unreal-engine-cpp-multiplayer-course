#include "GAS/CActionAbility_ProjectileAttack.h"

#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// This class is tightly coupled to the CCharacter class itself, which isn't ideal. This is going to stay like this for the time being and I might change it in the future. 
UCActionAbility_ProjectileAttack::UCActionAbility_ProjectileAttack()
{
	AttackSocketName = "Muzzle_01";
	AttackAnimationMontageNotifyStart = "AttackReady";
	AttackAnimationMontageNotifyEnd = "AttackDone";
}

void UCActionAbility_ProjectileAttack::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	ACharacter* InstigatorCharacter = Cast<ACharacter>(Instigator);
	if (!ensure(InstigatorCharacter)) return;

	InstigatorCharacter->PlayAnimMontage(AttackAnimation);
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlashParticleSystem, InstigatorCharacter->GetMesh(), InstigatorCharacter->GetMesh()->GetSocketBoneName(AttackSocketName));

	if (UAnimInstance* AnimationInstance = InstigatorCharacter->GetMesh()->GetAnimInstance())
	{
		AnimationInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &UCActionAbility_ProjectileAttack::OnMontageNotifyBegin);
	}
}

void UCActionAbility_ProjectileAttack::SpawnProjectile(ACharacter* InstigatorCharacter) const
{
	if (InstigatorCharacter->HasAuthority()) // Only spawn the projectile on the server. The projectile itself however, is a replicated Actor.
	{
		const FTransform SpawnTransform = FTransform(TraceForProjectileSpawnRotator(InstigatorCharacter), InstigatorCharacter->GetMesh()->GetSocketLocation(AttackSocketName));
		FActorSpawnParameters SpawnParameters = FActorSpawnParameters();
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParameters.Owner = InstigatorCharacter;
		SpawnParameters.Instigator = InstigatorCharacter;

		GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTransform, SpawnParameters);
	}
}

/**
 * @brief Traces for the rotation to be used when spawning a projectile.
 *
 * @details This method performs a line trace from Character's view point to find the target location for spawning a projectile.
 * It checks for collisions with specified object types and returns the rotation required to face the target location.
 * If the line trace does not result in a blocking hit, the rotation will be calculated based on the end location of the trace.
 *
 * @return The rotation to be used when spawning a projectile.
 */
FRotator UCActionAbility_ProjectileAttack::TraceForProjectileSpawnRotator(ACharacter* InstigatorCharacter) const
{
	FHitResult TraceHitResult;
	FVector TraceStart = InstigatorCharacter->GetPawnViewLocation();
	FVector TraceEnd = InstigatorCharacter->GetPawnViewLocation() + InstigatorCharacter->GetControlRotation().Vector() * 5000;
	FCollisionObjectQueryParams QueryParams = FCollisionObjectQueryParams();
	FCollisionQueryParams TraceParams(FName("Actor Self-Ignore trace parameter"), true, InstigatorCharacter);

	QueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	QueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
	QueryParams.AddObjectTypesToQuery(ECC_Pawn);
	QueryParams.AddObjectTypesToQuery(ECC_PhysicsBody);

	bool bIsTraceBlockingHit = GetWorld()->LineTraceSingleByObjectType(TraceHitResult, TraceStart, TraceEnd, QueryParams, TraceParams);
	FVector SpawnRotatorTarget = bIsTraceBlockingHit ? TraceHitResult.ImpactPoint : TraceEnd; // Handle cases where the tracing did not result in a blocking hit.
	return UKismetMathLibrary::FindLookAtRotation(InstigatorCharacter->GetMesh()->GetSocketLocation(AttackSocketName), SpawnRotatorTarget);
}

// I think the way the casting from the Payload is done is a nasty hack, but the result is still somehow better than using flat Timers instead of AnimNotifies. 
void UCActionAbility_ProjectileAttack::OnMontageNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	ACharacter* InstigatorCharacter = Cast<ACharacter>(BranchingPointPayload.SkelMeshComponent->GetOwner());
	if (!ensureAlways(InstigatorCharacter)) return;

	UAnimInstance* AnimationInstance = InstigatorCharacter->GetMesh()->GetAnimInstance();
	if (!ensureAlways(AnimationInstance)) return;

	if (NotifyName == AttackAnimationMontageNotifyStart)
	{
		SpawnProjectile(InstigatorCharacter);
	}
	else if (NotifyName == AttackAnimationMontageNotifyEnd)
	{
		AnimationInstance->OnPlayMontageNotifyBegin.RemoveDynamic(this, &UCActionAbility_ProjectileAttack::OnMontageNotifyBegin);
		StopAction(InstigatorCharacter);
	}
}
