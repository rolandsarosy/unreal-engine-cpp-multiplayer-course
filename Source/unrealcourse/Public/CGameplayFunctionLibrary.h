#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CGameplayFunctionLibrary.generated.h"

UCLASS()
class UNREALCOURSE_API UCGameplayFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Gameplay")
	static bool ApplyDamage(AActor* DamageInstigator, AActor* TargetActor, float DamageAmount);

	UFUNCTION(BlueprintCallable, Category="Gameplay")
	static bool ApplyDirectionalImpulseDamage(AActor* DamageInstigator, AActor* TargetActor, float DamageAmount, const FHitResult& HitResult);

	UFUNCTION(BlueprintCallable, Category="Gameplay")
	static bool ApplyHealing(AActor* HealingInstigator, AActor* TargetActor, float HealAmount);
};
