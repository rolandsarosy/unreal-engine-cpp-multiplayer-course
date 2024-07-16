#include "Projectiles/CBlackHoleProjectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"

ACBlackHoleProjectile::ACBlackHoleProjectile()
{
	RadialForceComponent = CreateDefaultSubobject<URadialForceComponent>("RadialForceComponent");
	RadialForceComponent->SetupAttachment(SphereComponent);
	RadialForceComponent->Radius = 550.0f;
	RadialForceComponent->ForceStrength = -500000.0f;
	RadialForceComponent->bIgnoreOwningActor = true;

	ProjectileMovementComponent->InitialSpeed = 1000;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;

	// This is fucking weird.
	RadialForceComponent->RemoveObjectTypeToAffect(UCollisionProfile::Get()->ConvertToObjectType(ECC_Pawn));
}
