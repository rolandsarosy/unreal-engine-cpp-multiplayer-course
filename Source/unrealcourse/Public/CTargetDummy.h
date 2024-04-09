#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CTargetDummy.generated.h"

class UCAttributeComponent;

UCLASS()
class UNREALCOURSE_API ACTargetDummy : public AActor
{
	GENERATED_BODY()

public:
	ACTargetDummy();


protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCAttributeComponent> AttributeComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;
	
	UFUNCTION()
	void OnHealthChanged(AActor* Actor, UCAttributeComponent* UAttributeComponent, float NewHealth, float Delta);

private:
	virtual void PostInitializeComponents() override;
};
