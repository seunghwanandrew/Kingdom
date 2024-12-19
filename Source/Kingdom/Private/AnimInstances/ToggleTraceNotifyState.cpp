#include "AnimInstances/ToggleTraceNotifyState.h"
#include "Character/PlayerClass.h"
#include "Combat/TraceComponent.h"

void UToggleTraceNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	APlayerClass* PlayerRef = Cast<APlayerClass>(MeshComp->GetOwner());
	if (!IsValid(PlayerRef)) return;
	UTraceComponent* Trace = PlayerRef->GetCurrentEquipWeaponTraceComponent();
	if (!Trace) return;
	Trace->IsAttacking(true);
}

void UToggleTraceNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	APlayerClass* PlayerRef = Cast<APlayerClass>(MeshComp->GetOwner());
	if (!IsValid(PlayerRef)) return;
	UTraceComponent* Trace = PlayerRef->GetCurrentEquipWeaponTraceComponent();
	if (!Trace) return;
	Trace->IsAttacking(false);
}
