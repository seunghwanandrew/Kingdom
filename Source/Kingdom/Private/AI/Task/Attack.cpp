#include "AI/Task/Attack.h"
#include "AIController.h"
#include "Character/EnemyClass.h"

EBTNodeResult::Type UAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemyClass* EnemyRef = Cast<AEnemyClass>(OwnerComp.GetAIOwner()->GetPawn());

	EnemyRef->Attack();

	return EBTNodeResult::Succeeded;
}
