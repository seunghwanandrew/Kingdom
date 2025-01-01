#include "UserWidget/EnemyHealthBar.h"
#include "Components/ProgressBar.h"

void UEnemyHealthBar::SetHealthBar(float Percent)
{
	HealthBar->SetPercent(Percent);
}
