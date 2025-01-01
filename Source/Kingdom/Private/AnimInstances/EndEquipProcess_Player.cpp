#include "AnimInstances/EndEquipProcess_Player.h"
#include "AnimInstances/PlayerAnimInstance.h"
#include "Character/PlayerClass.h"

void UEndEquipProcess_Player::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AActor* Owner = MeshComp->GetOwner();
	if (Cast<APlayerClass>(Owner))
	{
		APlayerClass* PlayerRef = Cast<APlayerClass>(Owner);
		UPlayerAnimInstance* AnimInstance = Cast<UPlayerAnimInstance>(MeshComp->GetAnimInstance());
		AnimInstance->SetActionState(EPlayerActionState::EPAS_Combat);
		PlayerRef->SetActionState(EPlayerActionState::EPAS_Combat);
	}
}
