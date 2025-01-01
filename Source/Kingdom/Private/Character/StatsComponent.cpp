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