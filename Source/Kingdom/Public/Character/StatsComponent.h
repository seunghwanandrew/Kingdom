#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnumClass/EnumClass.h"
#include "StatsComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class KINGDOM_API UStatsComponent : public UActorComponent
{
	GENERATED_BODY()

#pragma region Variables
public:
	UPROPERTY(EditAnywhere, Category = "Character Stats")
	TMap<EStats, float> Stats;
protected:
private:
	UPROPERTY(EditAnywhere, Category = "Character Stats", meta = (AllowPrivateAccess = "true"))
	double StaminaReGenRate = 30.0f;

	UPROPERTY(VisibleAnywhere, Category = "Character Stats", meta = (AllowPrivateAccess = "true"))
	bool bCanRegenerateStamina = true;

	UPROPERTY(EditAnywhere, Category = "Character Stats", meta = (AllowPrivateAccess = "true"))
	float StaminaDelayDuration = 2.0f;
#pragma endregion 

#pragma region Functions
public:	
	UStatsComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void ApplyDamage(float DamageAmount);
	void ApplyStaminaUsage(float UsageAmount);
	void StaminaRegenerating(float DeltaTime);
	
	UFUNCTION(BlueprintAuthorityOnly)
	void EnableReGeneration();
protected:
	virtual void BeginPlay() override;
private:
#pragma endregion		
};
