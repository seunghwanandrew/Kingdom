#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EnemyInterface.generated.h"

UINTERFACE(MinimalAPI)
class UEnemyInterface : public UInterface
{
	GENERATED_BODY()
};


class KINGDOM_API IEnemyInterface
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintNativeEvent)
	void OnTargeted();
	UFUNCTION(BlueprintNativeEvent)
	void UnTargeted();
};


