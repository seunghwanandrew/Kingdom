#include "UserWidget/UserHUD.h"
#include "UserWidget/PlayerDisplay.h"
#include "GameFrameWork/PlayerController.h"
#include "Character/PlayerClass.h"
#include "Character/StatsComponent.h"

AUserHUD::AUserHUD()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AUserHUD::BeginPlay()
{
	Super::BeginPlay();
	GetUserController();
	WidgetCreation();
}

void AUserHUD::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	WidgetUpdate();
}

void AUserHUD::GetUserController()
{
	if (IsValid(GetWorld()))
	{
		Controller = GetWorld()->GetFirstPlayerController();
		APlayerClass* Player = Cast<APlayerClass>(Controller->GetPawn());
		if (!IsValid(Player)) return;
		StatsComponent = Player->GetStatsComponent();
	}
}

void AUserHUD::WidgetCreation()
{
	if (!IsValid(Controller)) return;
	HUD = CreateWidget<UPlayerDisplay>(Controller, HeadUpDisplay);
	HUD->AddToViewport();
}

void AUserHUD::WidgetUpdate()
{	
	if (!IsValid(HUD) || !IsValid(StatsComponent)) return;
	HUD->SetHealthBar(StatsComponent->GetCurrentHealthPercentage());
	HUD->SetStaminaBar(StatsComponent->GetCurrentStaminaPercentage());
}
