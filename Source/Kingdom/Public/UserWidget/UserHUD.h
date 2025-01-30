#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UserHUD.generated.h"

class UPlayerDisplay;
class APlayerController;
class UStatsComponent;
class UCombatComponent;

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
	UPROPERTY(VisibleAnywhere)
	UCombatComponent* CombatComponent;
#pragma endregion
#pragma region Functions
public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE UPlayerDisplay* GetHUDWidget() { return HUD; }

	AUserHUD();
	virtual void BeginPlay() override;
	void WidgetCreation();
	void WidgetUpdate();

	UFUNCTION(BlueprintCallable)
	void WidgetVisibility(UPlayerDisplay* Displayer, ESlateVisibility Value);
protected:
	virtual void Tick(float DeltaTime) override;
private:
	void GetUserController();

#pragma endregion	
};
