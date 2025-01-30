#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WidgetScreenButton.generated.h"

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE(FOnScreenButtonClickSignature, UWidgetScreenButton, OnScreenButtonClickDelegate);

class USizeBox;
class UOverlay;
class UButton;
class UTextBlock;

UCLASS()
class KINGDOM_API UWidgetScreenButton : public UUserWidget
{
	GENERATED_BODY()

#pragma region Varaibles
public:
	FOnScreenButtonClickSignature OnScreenButtonClickDelegate;

	UPROPERTY(meta = (BindWidget))
	USizeBox* SizeBoxBase;
	UPROPERTY(meta = (BindWidget))
	UOverlay* OverlayBase;
	UPROPERTY(meta = (BindWidget))
	UButton* ButtonBase;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextBlock;
protected:
	UPROPERTY(BlueprintReadWrite, Category = InputValue);
	float Width = 500.0f;
	UPROPERTY(BlueprintReadWrite, Category = InputValue);
	float Height = 100.0f;
	UPROPERTY(BlueprintReadWrite, Category = InputValue);
	FText ButtonText;
private:
#pragma endregion

#pragma region Functions
public:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	UFUNCTION(BlueprintAuthorityOnly)
	void OnButtonClicked();

	void SetText(FName Text);
protected:
private:
#pragma endregion
};
