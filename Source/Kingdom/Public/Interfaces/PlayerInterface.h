#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PlayerInterface.generated.h"

UINTERFACE(MinimalAPI)
class UPlayerInterface : public UInterface
{
	GENERATED_BODY()
};

class KINGDOM_API IPlayerInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	void SetOverlappedItem(AActor* OverlappedItemRef);
	UFUNCTION(BlueprintNativeEvent)
	bool HasEnoughStamina(float MinValue);
};
