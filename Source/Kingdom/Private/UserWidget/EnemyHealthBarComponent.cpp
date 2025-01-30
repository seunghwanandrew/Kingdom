#include "UserWidget/EnemyHealthBarComponent.h"
#include "UserWidget/EnemyHealthBar.h"

void UEnemyHealthBarComponent::SetHealthWidget(float Percent)
{
	HealthBarWidget = Cast<UEnemyHealthBar>(GetUserWidgetObject());
	HealthBarWidget->SetHealthBar(Percent);
}
