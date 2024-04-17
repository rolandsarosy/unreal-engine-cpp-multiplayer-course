#include "AI/CBTTask_RangedAttack.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"

EBTNodeResult::Type UCBTTask_RangedAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ACharacter* AICharacter = GetAICharacter(OwnerComp);
	AActor* TargetActor = GetTargetActor(OwnerComp);

	if (!AICharacter || !TargetActor) return EBTNodeResult::Failed;

	FVector MuzzleLocation = AICharacter->GetMesh()->GetSocketLocation(AttackSocketName);
	FRotator MuzzleRotation = UKismetMathLibrary::FindLookAtRotation(MuzzleLocation, TargetActor->GetActorLocation());

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParameters.Instigator = AICharacter;

	AActor* SpawnedProjectile = GetWorld()->SpawnActor<AActor>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParameters);

	return SpawnedProjectile ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
}

AActor* UCBTTask_RangedAttack::GetTargetActor(const UBehaviorTreeComponent& OwnerComp) const
{
	const UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComponent) return nullptr;

	return Cast<AActor>(BlackboardComponent->GetValueAsObject(TargetActorKey.SelectedKeyName));;
}

ACharacter* UCBTTask_RangedAttack::GetAICharacter(const UBehaviorTreeComponent& OwnerComp)
{
	const AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController) return nullptr;

	return Cast<ACharacter>(AIController->GetPawn());
}
