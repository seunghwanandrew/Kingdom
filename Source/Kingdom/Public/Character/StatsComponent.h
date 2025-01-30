#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnumClass/EnumClass.h"
#include "StatsComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(FOnDeathSignature, UStatsComponent, OnDeathDelegate, int32, Index);

class UAnimMontage;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class KINGDOM_API UStatsComponent : public UActorComponent
{
	GENERATED_BODY()

#pragma region Variables
public:
	FOnDeathSignature OnDeathDelegate;

	UPROPERTY(EditAnywhere, Category = "Character Stats")
	TMap<EStats, float> Stats;
protected:
	UPROPERTY(EditAnywhere, Category = Montage)
	TArray<UAnimMontage*> DeathMontageList;
	UPROPERTY(EditAnywhere, Category = Montage)
	UAnimMontage* DeathMontage;

private:
	UPROPERTY(EditAnywhere, Category = "Character Stats", meta = (AllowPrivateAccess = "true"))
	double StaminaReGenRate = 5.0f;
	UPROPERTY(VisibleAnywhere, Category = "Character Stats", meta = (AllowPrivateAccess = "true"))
	bool bCanRegenerateStamina = true;
	UPROPERTY(EditAnywhere, Category = "Character Stats", meta = (AllowPrivateAccess = "true"))
	float StaminaDelayDuration = 2.0f;

	FTimerHandle HealthRegenTimer;
	bool bIsHealthRegenerating = false;
	float TargetHealthReGenAmount;
#pragma endregion 

#pragma region Functions
public:	
	/* Getter */
	FORCEINLINE float GetCurrentHealthPercentage() { return Stats[EStats::ES_Health] / Stats[EStats::ES_MaxHealth]; }
	FORCEINLINE float GetCurrentStaminaPercentage() { return Stats[EStats::ES_Stamina] / Stats[EStats::ES_MaxStamina]; }

	UStatsComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void ApplyDamage(float DamageAmount);
	void ApplyStaminaUsage(float UsageAmount);
	void StaminaRegenerating(float DeltaTime);
	void StartHealthReGen(float Amount);
	void HealthRegenerateProcess();
	void DeathProcess();
	UFUNCTION(BlueprintAuthorityOnly)
	void DeathNotify(int32 Index);
	
	UFUNCTION(BlueprintAuthorityOnly)
	void EnableReGeneration();
protected:
	virtual void BeginPlay() override;
private:
#pragma endregion		
};
