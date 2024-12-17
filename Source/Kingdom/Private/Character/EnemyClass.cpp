#include "Character/EnemyClass.h"
#include "Components/CapsuleComponent.h"
#include "UserWidget/TargetDisplayerWidgetComponent.h"

AEnemyClass::AEnemyClass()
{
	PrimaryActorTick.bCanEverTick = true;
	Initialize();
	CreateInitComponent();
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

