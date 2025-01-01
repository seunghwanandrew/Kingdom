#include "AnimInstances/EndEngaging_Player.h"
#include "Character/PlayerClass.h"

void UEndEngaging_Player::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AActor* Owner = MeshComp->GetOwner();
	if (Cast<APlayerClass>(Owner))
	{
		APlayerClass* PlayerRef = Cast<APlayerClass>(Owner);
		PlayerRef->SetIsEngaging(false);
	}
}
