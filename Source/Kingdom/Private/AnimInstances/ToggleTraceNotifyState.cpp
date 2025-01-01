#include "AnimInstances/ToggleTraceNotifyState.h"
#include "Character/PlayerClass.h"
#include "Character/EnemyClass.h"
#include "Combat/TraceComponent.h"

void UToggleTraceNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	APlayerClass* PlayerRef = Cast<APlayerClass>(MeshComp->GetOwner());
	if (IsValid(PlayerRef))
	{
		UTraceComponent* Trace = PlayerRef->GetCurrentEquipWeaponTraceComponent();
		if (!Trace) return;
		Trace->IsAttacking(true);
	}
	AEnemyClass* EnemyRef = Cast<AEnemyClass>(MeshComp->GetOwner());
	if (IsValid(EnemyRef))
	{
		UTraceComponent* Trace = nullptr;
		if (EnemyRef->HasDefaultWeapon())
		{
			Trace = EnemyRef->GetCurrentEquipWeaponTraceComponent();
		}
		else
		{
			Trace = EnemyRef->GetTraceComponent();
		}
		if (!Trace) return;
		Trace->IsAttacking(true);
	}
}

void UToggleTraceNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	APlayerClass* PlayerRef = Cast<APlayerClass>(MeshComp->GetOwner());
	if (IsValid(PlayerRef))
	{
		UTraceComponent* Trace = PlayerRef->GetCurrentEquipWeaponTraceComponent();
		if (!Trace) return;
		Trace->IsAttacking(false);
	}
	AEnemyClass* EnemyRef = Cast<AEnemyClass>(MeshComp->GetOwner());
	if (IsValid(EnemyRef))
	{
		UTraceComponent* Trace = nullptr;
		if (EnemyRef->HasDefaultWeapon())
		{
			Trace = EnemyRef->GetCurrentEquipWeaponTraceComponent();
		}
		else
		{
			Trace = EnemyRef->GetTraceComponent();
		}
		if (!Trace) return;
		Trace->IsAttacking(false);
	}
}
