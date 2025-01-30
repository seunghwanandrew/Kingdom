#include "AI/Task/Attack.h"
#include "AIController.h"
#include "Character/EnemyClass.h"
#include "Character/BossEnemyClass.h"

EBTNodeResult::Type UAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemyClass* EnemyRef;
	if (Cast<ABossEnemyClass>(OwnerComp.GetAIOwner()->GetPawn()))
	{
		EnemyRef = Cast<ABossEnemyClass>(OwnerComp.GetAIOwner()->GetPawn());
	}
	else
	{
		EnemyRef = Cast<AEnemyClass>(OwnerComp.GetAIOwner()->GetPawn());
	}

	EnemyRef->Attack();

	return EBTNodeResult::Succeeded;
}
