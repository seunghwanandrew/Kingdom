#include "AnimInstances/EndUnEquipProcess_Player.h"
#include "Character/PlayerClass.h"
#include "AnimInstances/PlayerAnimInstance.h"

void UEndUnEquipProcess_Player::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AActor* Owner = MeshComp->GetOwner();
	if (Cast<APlayerClass>(Owner))
	{
		APlayerClass* PlayerRef = Cast<APlayerClass>(Owner);
		UPlayerAnimInstance* AnimInstance = Cast<UPlayerAnimInstance>(MeshComp->GetAnimInstance());
		AnimInstance->SetActionState(EPlayerActionState::EPAS_None);
		PlayerRef->SetActionState(EPlayerActionState::EPAS_None);
	}	
}
