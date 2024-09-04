#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/CInteractableInterface.h"
#include "Interfaces/CSaveableInterface.h"
#include "CItemChest.generated.h"

UCLASS(Abstract)
class UNREALCOURSE_API ACItemChest : public AActor, public ICInteractableInterface, public ICSaveableInterface
{
	GENERATED_BODY()

public:
	ACItemChest();

	virtual FText GetInteractText_Implementation(APawn* InstigatorPawn) override;

protected:
	UPROPERTY(VisibleAnywhere, Category="ItemChest")
	TObjectPtr<UStaticMeshComponent> BaseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="ItemChest")
	TObjectPtr<UStaticMeshComponent> LidMesh;

	UPROPERTY(ReplicatedUsing="OnRep_IsLidOpened", BlueprintReadOnly, Category="ItemChest", SaveGame)
	bool bIsLidOpened;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category="ItemChest", SaveGame)
	bool bIsLocked;

	UFUNCTION(BlueprintNativeEvent, Category="ItemChest")
	void OnRep_IsLidOpened();

	UFUNCTION(BlueprintNativeEvent, Category="ItemChest")
	bool AttemptUnlock(AActor* InstigatorActor);

	UFUNCTION(NetMulticast, NotBlueprintable, Unreliable, Category="ItemChest")
	void MulticastOnUnSuccessfulOpeningAttempt();

	UFUNCTION(BlueprintNativeEvent, Category="ItemChest")
	void OnUnsuccessfullOpeningAttempt();

private:
	virtual void OnActorLoaded_Implementation() override;

	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
};
