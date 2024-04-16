#include "AI/CAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

void ACAIController::BeginPlay()
{
	Super::BeginPlay();

	RunBehaviorTree(BehaviorTree);

	if (APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0))
	{
		GetBlackboardComponent()->SetValueAsVector("MoveToLocation", PlayerPawn->GetActorLocation()); // Temporary stuff.
		GetBlackboardComponent()->SetValueAsObject("TargetActor", PlayerPawn);
	}
}
