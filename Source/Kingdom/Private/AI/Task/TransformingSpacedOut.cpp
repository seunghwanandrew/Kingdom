#include "AI/Task/TransformingSpacedOut.h"
#include "AIController.h"
#include "AI/Controller/EnemyController.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Character/EnemyClass.h"

UTransformingSpacedOut::UTransformingSpacedOut()
{
	NodeName = "Transforming Spaced Out";
	// instantiating to be able to use Timers
	bCreateNodeInstance = true;

	bLooping = false;
	bNonBlocking = false;

	TimerDelegate = FTimerDelegate::CreateUObject(this, &UTransformingSpacedOut::OnAnimationTimerDone);

	PreviousAnimationMode = EAnimationMode::AnimationBlueprint;
}

EBTNodeResult::Type UTransformingSpacedOut::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* const ControllerRef = OwnerComp.GetAIOwner();
	EBTNodeResult::Type Result = EBTNodeResult::Failed;
	AEnemyClass* EnemyRef = Cast<AEnemyClass>(ControllerRef->GetPawn());

	// reset timer handle
	TimerHandle.Invalidate();
	OwnerComponent = &OwnerComp;

	AnimToPlay = GetMontageToPlay(EnemyRef);

	if (AnimToPlay && ControllerRef && ControllerRef->GetPawn())
	{
		USkeletalMeshComponent* SkelMesh = nullptr;
		ACharacter* const CharacterRef = Cast<ACharacter>(ControllerRef->GetPawn());
		if (CharacterRef)
		{
			SkelMesh = CharacterRef->GetMesh();
		}
		else
		{
			SkelMesh = ControllerRef->GetPawn()->FindComponentByClass<USkeletalMeshComponent>();
		}

		if (SkelMesh != nullptr)
		{
			PreviousAnimationMode = SkelMesh->GetAnimationMode();
			CachedSkelMesh = SkelMesh;

			SkelMesh->PlayAnimation(AnimToPlay, bLooping.GetValue(OwnerComp));

			const float FinishDelay = AnimToPlay->GetPlayLength();

			if (bNonBlocking.GetValue(OwnerComp) == false && FinishDelay > 0)
			{
				if (bLooping.GetValue(OwnerComp) == false)
				{
					ControllerRef->GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, FinishDelay, /*bLoop=*/false);
				}
				Result = EBTNodeResult::InProgress;
			}
			else
			{
				UE_CVLOG(bNonBlocking.GetValue(OwnerComp) == false, ControllerRef, LogBehaviorTree, Log, TEXT("%s> Instant success due to having a valid AnimationToPlay and Character with SkelMesh, but 0-length animation"), *GetNodeName());
				// we're done here, report success so that BT can pick next task
				Result = EBTNodeResult::Succeeded;
			}
		}
	}

	return Result;
}

EBTNodeResult::Type UTransformingSpacedOut::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* const Controller = OwnerComp.GetAIOwner();

	if (AnimToPlay && Controller && TimerHandle.IsValid())
	{
		Controller->GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	}

	TimerHandle.Invalidate();

	CleanUp(OwnerComp);

	return EBTNodeResult::Aborted;
}

UAnimMontage* UTransformingSpacedOut::GetMontageToPlay(AEnemyClass* Ref)
{
	return Ref->GetTransformPlayMontage();
}

void UTransformingSpacedOut::OnAnimationTimerDone()
{
	if (OwnerComponent)
	{
		CleanUp(*OwnerComponent);
		FinishLatentTask(*OwnerComponent, EBTNodeResult::Succeeded);
	}
}

void UTransformingSpacedOut::CleanUp(UBehaviorTreeComponent& OwnerComp)
{
	if (CachedSkelMesh != nullptr && PreviousAnimationMode == EAnimationMode::AnimationBlueprint)
	{
		CachedSkelMesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	}
}
