#include "UserWidget/PlayerDisplay.h"
#include "Components/ProgressBar.h"

void UPlayerDisplay::SetHealthBar(float Percent)
{
	if (!IsValid(HealthBar)) return;
	HealthBar->SetPercent(Percent);
}

void UPlayerDisplay::SetStaminaBar(float Percent)
{
	if (!IsValid(StaminaBar)) return;
	StaminaBar->SetPercent(Percent);
}
