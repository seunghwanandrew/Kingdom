#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Attack.generated.h"


UCLASS()
class KINGDOM_API UAttack : public UBTTaskNode
{
	GENERATED_BODY()
	
#pragma region Variables
public:
protected:
private:
#pragma endregion

#pragma region Functions
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
protected:
private:
#pragma endregion
};
