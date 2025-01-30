#include "Character/BossEnemyClass.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UserWidget/TargetDisplayerWidgetComponent.h"

ABossEnemyClass::ABossEnemyClass() : AEnemyClass()
{
	TargetDisplayer = CreateDefaultSubobject<UTargetDisplayerWidgetComponent>(TEXT("TargetDisplayer"));
	TargetDisplayer->SetupAttachment(GetRootComponent());
	TargetDisplayer->SetWidgetSpace(EWidgetSpace::Screen);
	TargetDisplayer->SetVisibility(false);

	InitWalkSpeed = 450.0f;
	MaxWalkSpeed = 450.0f;
	GetCharacterMovement()->MaxWalkSpeed = 450.0f;
	Tags.Add(FName("Boss"));
}

void ABossEnemyClass::BeginPlay()
{
	Super::BeginPlay();
}

void ABossEnemyClass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABossEnemyClass::OnTargeted_Implementation()
{
	TargetDisplayer->SetVisibility(true);
	HealthBarComponent->SetVisibility(true);
}

void ABossEnemyClass::UnTargeted_Implementation()
{
	TargetDisplayer->SetVisibility(false);
	HealthBarComponent->SetVisibility(false);
}