#include "AI/CBTTask_RangedAttack.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"

EBTNodeResult::Type UCBTTask_RangedAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ACharacter* AICharacter = GetAICharacter(OwnerComp);
	const AActor* TargetActor = GetTargetActor(OwnerComp);

	if (!AICharacter || !TargetActor) return EBTNodeResult::Failed;

	const FVector MuzzleLocation = AICharacter->GetMesh()->GetSocketLocation(AttackSocketName);
	const FRotator MuzzleRotation = UKismetMathLibrary::FindLookAtRotation(MuzzleLocation, TargetActor->GetActorLocation());

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParameters.Instigator = AICharacter;

	const AActor* SpawnedProjectile = GetWorld()->SpawnActor<AActor>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParameters);

	return SpawnedProjectile ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
}

AActor* UCBTTask_RangedAttack::GetTargetActor(const UBehaviorTreeComponent& OwnerComp) const
{
	const UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	if (!ensure(BlackboardComponent)) return nullptr;

	return Cast<AActor>(BlackboardComponent->GetValueAsObject(TargetActorKey.SelectedKeyName));
}

// Casting to ACharacter instead of APawn is necessary to be able to get the Mesh later on.
ACharacter* UCBTTask_RangedAttack::GetAICharacter(const UBehaviorTreeComponent& OwnerComp)
{
	const AAIController* AIController = OwnerComp.GetAIOwner();
	if (!ensure(AIController)) return nullptr;

	return Cast<ACharacter>(AIController->GetPawn());
}
