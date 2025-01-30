#include "Character/StatsComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/Character.h"

UStatsComponent::UStatsComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UStatsComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UStatsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UStatsComponent::ApplyDamage(float DamageAmount)
{
	Stats[EStats::ES_Health] = FMath::Clamp(Stats[EStats::ES_Health], 0.0f, Stats[EStats::ES_Health] - DamageAmount);

	if (Stats[EStats::ES_Health] <= 0.0f)
	{
		DeathProcess();
	}
}

void UStatsComponent::ApplyStaminaUsage(float UsageAmount)
{
	Stats[EStats::ES_Stamina] = FMath::Clamp(Stats[EStats::ES_Stamina], 0.0f, Stats[EStats::ES_Stamina] - UsageAmount);

	bCanRegenerateStamina = false;

	FLatentActionInfo FunctionInfo { 0, 100, TEXT("EnableReGeneration"), this };
	UKismetSystemLibrary::RetriggerableDelay(GetWorld(), StaminaDelayDuration, FunctionInfo);
}

void UStatsComponent::StaminaRegenerating(float DeltaTime)
{
	if (!bCanRegenerateStamina) return;
	Stats[EStats::ES_Stamina] = UKismetMathLibrary::FInterpTo_Constant(Stats[EStats::ES_Stamina], Stats[EStats::ES_MaxStamina], DeltaTime, StaminaReGenRate);
}

void UStatsComponent::StartHealthReGen(float Amount)
{
	if (bIsHealthRegenerating) return;

	bIsHealthRegenerating = true;

	TargetHealthReGenAmount = FMath::Min(Stats[EStats::ES_Health] + Amount, Stats[EStats::ES_MaxHealth]);

	GetWorld()->GetTimerManager().SetTimer(HealthRegenTimer, this, &UStatsComponent::HealthRegenerateProcess, 0.01f, true);
}

void UStatsComponent::HealthRegenerateProcess()
{
	if (!bIsHealthRegenerating) return;

	float DeltaTime = GetWorld()->GetDeltaSeconds();
	float HealthRegenRate = 5.0f;
	Stats[EStats::ES_Health] = UKismetMathLibrary::FInterpTo_Constant(
		Stats[EStats::ES_Health],
		TargetHealthReGenAmount,
		DeltaTime,
		HealthRegenRate
	);

	if (FMath::IsNearlyEqual(Stats[EStats::ES_Health], TargetHealthReGenAmount, 0.01f))
	{
		Stats[EStats::ES_Health] = TargetHealthReGenAmount;
		bIsHealthRegenerating = false;
		GetWorld()->GetTimerManager().ClearTimer(HealthRegenTimer);
	}
}

void UStatsComponent::DeathProcess()
{
	if (DeathMontageList.Num() <= 0) return;

	int32 RandomIndex = FMath::RandRange(0, DeathMontageList.Num() - 1);
	DeathMontage = DeathMontageList[RandomIndex];
	Cast<ACharacter>(GetOwner())->PlayAnimMontage(DeathMontage);
	float PlayTime = DeathMontage->GetPlayLength() - 0.5f;
	FTimerHandle DelayTimer;
	FTimerDelegate Notify;
	Notify.BindUFunction(this, FName("DeathNotify"), RandomIndex);
	GetWorld()->GetTimerManager().SetTimer(DelayTimer, Notify, PlayTime, false);
}

void UStatsComponent::DeathNotify(int32 Index)
{
	OnDeathDelegate.Broadcast(Index);
}

void UStatsComponent::EnableReGeneration()
{
	bCanRegenerateStamina = true;
}