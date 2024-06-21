#pragma once

#include "CoreMinimal.h"
#include "CGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "CItemChest.generated.h"

UCLASS(Abstract)
class UNREALCOURSE_API ACItemChest : public AActor, public ICGameplayInterface
{
	GENERATED_BODY()

	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

public:
	ACItemChest();

protected:
	UPROPERTY(VisibleAnywhere, Category="ItemChest")
	TObjectPtr<UStaticMeshComponent> BaseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="ItemChest")
	TObjectPtr<UStaticMeshComponent> LidMesh;

	UPROPERTY(Replicated, BlueprintReadOnly)
	bool IsLidOpened;
};
