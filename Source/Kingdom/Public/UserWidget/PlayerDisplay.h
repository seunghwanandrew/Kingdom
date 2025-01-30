#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerDisplay.generated.h"

class UProgressBar;
class UTextBlock;

UCLASS()
class KINGDOM_API UPlayerDisplay : public UUserWidget
{
	GENERATED_BODY()
	
#pragma region Variables
public:
protected:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;
	UPROPERTY(meta = (BindWidget))
	UProgressBar* StaminaBar;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* HPBarPercentDisplay;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* STBarPercentDisplay;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ArrowAmountDisplay;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* SoulAmountDisplay;
private:
#pragma endregion

#pragma region Functions
public:
	void SetHealthBar(float Percent);
	void SetStaminaBar(float Percent);
	void SetHealthTextBlock(float Percent);
	void SetStaminaTextBlock(float Percent);
	void SetArrowAmountDisplay(int32 Amount);
	void SetSoulAmountDisplay(int32 Amount);
protected:
private:
#pragma endregion
};
