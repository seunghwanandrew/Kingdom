#include "UserWidget/PlayerDisplay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Character/PlayerClass.h"

void UPlayerDisplay::SetHealthBar(float Percent)
{
	if (!IsValid(HealthBar)) return;
	HealthBar->SetPercent(Percent);
	SetHealthTextBlock(Percent * 100.0f);
}

void UPlayerDisplay::SetStaminaBar(float Percent)
{
	if (!IsValid(StaminaBar)) return;
	StaminaBar->SetPercent(Percent);
	SetStaminaTextBlock(Percent * 100.0f);
}

void UPlayerDisplay::SetHealthTextBlock(float Percent)
{
	FString PercentText = FString::Printf(TEXT("%.1f%%"), Percent);
	HPBarPercentDisplay->SetText(FText::FromString(PercentText));
}

void UPlayerDisplay::SetStaminaTextBlock(float Percent)
{
	FString PercentText = FString::Printf(TEXT("%.1f%%"), Percent);
	STBarPercentDisplay->SetText(FText::FromString(PercentText));
}

void UPlayerDisplay::SetArrowAmountDisplay(int32 Amount)
{
	FString Text = FString::Printf(TEXT("%i"), Amount);
	ArrowAmountDisplay->SetText(FText::FromString(Text));
}

void UPlayerDisplay::SetSoulAmountDisplay(int32 Amount)
{
	FString Text = FString::Printf(TEXT("%i"), Amount);
	SoulAmountDisplay->SetText(FText::FromString(Text));
}
