#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EnumClass/EnumClass.h"
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
	void StatsReCharge(float HealthReChargeValue);
	UFUNCTION(BlueprintNativeEvent)
	bool HasEnoughStamina(float MinValue);
	UFUNCTION(BlueprintNativeEvent)
	void TargetDeathProcess();
	UFUNCTION(BlueprintNativeEvent)
	void SetPlayerSoulOrItemAmount(int32 SoulAmount, int32 ItemAmount);
	UFUNCTION(BlueprintNativeEvent)
	EPlayerActionState GetPlayerActionState();
};
