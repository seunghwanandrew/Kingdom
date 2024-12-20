#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HitInterface.generated.h"

UINTERFACE(MinimalAPI)
class UHitInterface : public UInterface
{
	GENERATED_BODY()
};

class KINGDOM_API IHitInterface
{
	GENERATED_BODY()
public:
	virtual float GetDamage() { return 10.0f; }

	UFUNCTION(BlueprintNativeEvent)
	float GetCharacterStrength();
	UFUNCTION(BlueprintNativeEvent)
	void GetHit(float DamageAmount, AController* EventInstigator, AActor* DamageCauser);
};
