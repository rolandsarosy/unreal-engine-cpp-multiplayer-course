#include "Projectiles/CMagicProjectile.h"

#include "GameFramework/ProjectileMovementComponent.h"

ACMagicProjectile::ACMagicProjectile()
{
	ProjectileMovementComponent->InitialSpeed = 2000;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
}
