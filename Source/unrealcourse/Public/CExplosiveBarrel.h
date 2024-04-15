#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CExplosiveBarrel.generated.h"

class URadialForceComponent;

UCLASS()
class UNREALCOURSE_API AExplosiveBarrel : public AActor
{
	GENERATED_BODY()

public:
	AExplosiveBarrel();

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<URadialForceComponent> RadialForceComponent;

private:
	virtual void PostInitializeComponents() override;

	UFUNCTION()
	void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
