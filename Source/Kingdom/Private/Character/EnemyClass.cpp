#include "Character/EnemyClass.h"
#include "Components/CapsuleComponent.h"
#include "UserWidget/TargetDisplayerWidgetComponent.h"
#include "Engine/DamageEvents.h"
#include "Character/StatsComponent.h"

AEnemyClass::AEnemyClass()
{
	PrimaryActorTick.bCanEverTick = true;
	Initialize();
	CreateInitComponent();
	CreateCombatComponent();
}

void AEnemyClass::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemyClass::OnTargeted_Implementation()
{
	TargetDisplayer->SetVisibility(true);
}

void AEnemyClass::UnTargeted_Implementation()
{
	TargetDisplayer->SetVisibility(false);
}

void AEnemyClass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemyClass::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyClass::GetHit_Implementation(float DamageAmount, AController* EventInstigator, AActor* DamageCauser)
{
	// To Do Hit Reaction;
	FDamageEvent TargetAttackedEvent;
	float Damage = TakeDamage(DamageAmount, TargetAttackedEvent, EventInstigator, DamageCauser);
	
	StatsComponent->ApplyDamage(Damage);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::Printf(TEXT("Current Health : %f"), StatsComponent->Stats[EStats::ES_Health]));
	}
}

void AEnemyClass::Initialize()
{
	GetCapsuleComponent()->SetCapsuleSize(42.0f, 96.0f);	
}

void AEnemyClass::CreateInitComponent()
{
	TargetDisplayer = CreateDefaultSubobject<UTargetDisplayerWidgetComponent>(TEXT("TargetDisplayer"));
	TargetDisplayer->SetupAttachment(GetRootComponent());
	TargetDisplayer->SetWidgetSpace(EWidgetSpace::Screen);
	TargetDisplayer->SetVisibility(false);
}

void AEnemyClass::CreateCombatComponent()
{
	StatsComponent = CreateDefaultSubobject<UStatsComponent>(TEXT("Stats"));
}

