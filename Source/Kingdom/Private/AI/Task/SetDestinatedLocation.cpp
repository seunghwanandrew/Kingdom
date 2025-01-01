#include "AI/Task/SetDestinatedLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Character/EnemyClass.h"

EBTNodeResult::Type USetDestinatedLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EnemyBlackboard = OwnerComp.GetBlackboardComponent();
	AEnemyClass* Enemy = Cast<AEnemyClass>(OwnerComp.GetAIOwner()->GetPawn());

	if (Enemy)
	{
		CurrentLocation = Enemy->GetActorLocation();
		EnemyBlackboard->SetValueAsVector(FName("CurrentLocation"), CurrentLocation);

		DestinatedLocation = Enemy->RandomDestinationLocation();
		EnemyBlackboard->SetValueAsVector(FName("DestinatedLocation"), DestinatedLocation);
	}
	return EBTNodeResult::Succeeded;
}
