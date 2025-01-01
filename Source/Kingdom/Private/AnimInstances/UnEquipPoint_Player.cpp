#include "AnimInstances/UnEquipPoint_Player.h"
#include "Character/PlayerClass.h"

void UUnEquipPoint_Player::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AActor* Owner = MeshComp->GetOwner();
	if (Cast<APlayerClass>(Owner))
	{
		APlayerClass* PlayerRef = Cast<APlayerClass>(Owner);
		PlayerRef->UnEquipPoint();
	}
}
