#pragma once

#include "CoreMinimal.h"
#include "CGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "CItemChest.generated.h"

UCLASS()
class UNREALCOURSE_API ACItemChest : public AActor, public ICGameplayInterface
{
	GENERATED_BODY()

	void Interact_Implementation(APawn* InstigatorPawn) override;

public:
	ACItemChest();

protected:
	UPROPERTY(VisibleAnywhere, Category="ItemChest")
	TObjectPtr<UStaticMeshComponent> BaseMesh;

	UPROPERTY(VisibleAnywhere, Category="ItemChest")
	TObjectPtr<UStaticMeshComponent> LidMesh;

	UPROPERTY(EditAnywhere, Category="ItemChest")
	float ItemChestOpenTargetPitch;
	
	UPROPERTY(EditAnywhere, Category="ItemChest")
	float ItemChestClosedTargetPitch;
};
