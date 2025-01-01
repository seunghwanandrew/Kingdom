#include "AnimInstances/AttackEnd.h"
#include "Character/PlayerClass.h"
#include "Character/EnemyClass.h"
#include "Combat/TraceComponent.h"

void UAttackEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AActor* Owner = MeshComp->GetOwner();
	if (Cast<APlayerClass>(Owner))
	{
		APlayerClass* PlayerRef = Cast<APlayerClass>(Owner);
		PlayerRef->HandleAttackEnd();
	}
	else if (Cast<AEnemyClass>(Owner))
	{
		AEnemyClass* EnemyRef = Cast <AEnemyClass>(Owner);
		EnemyRef->HandleAttackEnd();
	}
}
