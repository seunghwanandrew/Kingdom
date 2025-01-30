#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "EnemyHealthBarComponent.generated.h"

class UEnemyHealthBar;

UCLASS()
class KINGDOM_API UEnemyHealthBarComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
public:
	UEnemyHealthBar* HealthBarWidget;
	void SetHealthWidget(float Percent);
};
