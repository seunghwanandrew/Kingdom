#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerDisplay.generated.h"

class UProgressBar;

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
private:
#pragma endregion

#pragma region Functions
public:
	void SetHealthBar(float Percent);
	void SetStaminaBar(float Percent);
protected:
private:
#pragma endregion
};
