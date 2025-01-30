#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "SetDestinatedLocation.generated.h"

UCLASS()
class KINGDOM_API USetDestinatedLocation : public UBTTaskNode
{
	GENERATED_BODY()
#pragma region Variables
public:
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBlackboardComponent* EnemyBlackboard;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector CurrentLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector DestinatedLocation;
private:
#pragma endregion

#pragma region Functions
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
protected:
private:
#pragma endregion	
};
