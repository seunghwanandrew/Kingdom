#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UserHUD.generated.h"

class UPlayerDisplay;
class APlayerController;
class UStatsComponent;

UCLASS()
class KINGDOM_API AUserHUD : public AHUD
{
	GENERATED_BODY()
#pragma region Variables
public:
protected:
private:
	UPROPERTY(EditDefaultsOnly, Category = HUD)
	TSubclassOf<UPlayerDisplay> HeadUpDisplay;
	UPROPERTY(VisibleAnywhere, Category = HUD)
	UPlayerDisplay* HUD;
	UPROPERTY(VisibleAnywhere)
	APlayerController* Controller;
	UPROPERTY(VisibleAnywhere)
	UStatsComponent* StatsComponent;
#pragma endregion
#pragma region Functions
public:
	AUserHUD();
	virtual void BeginPlay() override;
protected:
	virtual void Tick(float DeltaTime) override;
private:
	void GetUserController();
	void WidgetCreation();
	void WidgetUpdate();
#pragma endregion	
};
