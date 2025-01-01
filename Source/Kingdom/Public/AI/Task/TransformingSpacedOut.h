#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/ValueOrBBKey.h"
#include "TransformingSpacedOut.generated.h"

class AEnemyClass;
class AEnemyController;
class UAnimMontage;
struct FTimerHandle;

UCLASS()
class KINGDOM_API UTransformingSpacedOut : public UBTTaskNode
{
	GENERATED_BODY()
	
#pragma region Variables
public:
	/** Animation asset to play. Note that it needs to match the skeleton of pawn this BT is controlling */
	UPROPERTY(Category = Montage, EditAnywhere)
	UAnimMontage* AnimToPlay;

	UPROPERTY(Category = Node, EditAnywhere, DisplayName = Looping)
	FValueOrBBKey_Bool bLooping = false;
	/** if true the task will just trigger the animation and instantly finish. Fire and Forget. */
	UPROPERTY(Category = Node, EditAnywhere, DisplayName = NonBlocking)
	FValueOrBBKey_Bool bNonBlocking = false;

	UPROPERTY()
	TObjectPtr<USkeletalMeshComponent> CachedSkelMesh;

	EAnimationMode::Type PreviousAnimationMode;

	FTimerDelegate TimerDelegate;
	FTimerHandle TimerHandle;

	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> OwnerComponent;
protected:
private:

#pragma endregion

#pragma region Functions
public:
	UTransformingSpacedOut();
	virtual EBTNodeResult::Type	ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	UAnimMontage* GetMontageToPlay(AEnemyClass* Ref);
	void OnAnimationTimerDone();

protected:
	void CleanUp(UBehaviorTreeComponent& OwnerComp);
private:
#pragma endregion
};
