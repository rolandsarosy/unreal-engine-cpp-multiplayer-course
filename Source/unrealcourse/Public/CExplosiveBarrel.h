#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CExplosiveBarrel.generated.h"

class URadialForceComponent;

UCLASS(Abstract)
class UNREALCOURSE_API AExplosiveBarrel : public AActor
{
	GENERATED_BODY()

public:
	AExplosiveBarrel();

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<URadialForceComponent> RadialForceComponent;

	virtual void PostInitializeComponents() override;

	UFUNCTION()
	void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void OnComponentOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
