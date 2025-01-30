#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameMenuScreen.generated.h"

class UOverlay;
class UImage;
class UTextBlock;
class UWidgetScreenButton;
class UWidgetAnimation;

UCLASS()
class KINGDOM_API UGameMenuScreen : public UUserWidget
{
	GENERATED_BODY()
#pragma region Variables
public:
	UPROPERTY(meta = (BindWidget))
	UOverlay* OverlayBase;
	UPROPERTY(meta = (BindWidget))
	UImage* BackGround;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* MainText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* SubTitle;
	UPROPERTY(meta = (BindWidget))
	UWidgetScreenButton* Button1;
	UPROPERTY(meta = (BindWidget))
	UWidgetScreenButton* Button2;
	UPROPERTY(meta = (BindWidget))
	UWidgetScreenButton* Button3;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* AnimationToPlay;

	UPROPERTY(BlueprintReadWrite, Category = InputValue)
	FName MainTextName;
	UPROPERTY(BlueprintReadWrite, Category = InputValue)
	FName SubTitleName;
	UPROPERTY(BlueprintReadWrite, Category = InputValue)
	FName Button1Name;
	UPROPERTY(BlueprintReadWrite, Category = InputValue)
	FName Button2Name;
	UPROPERTY(BlueprintReadWrite, Category = InputValue)
	FName Button3Name;

protected:
private:
#pragma endregion

#pragma region Functions
public:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
protected:
	UFUNCTION(BlueprintImplementableEvent)
	void OnClickButton1();
	UFUNCTION(BlueprintImplementableEvent)
	void OnClickButton2();
	UFUNCTION(BlueprintImplementableEvent)
	void OnClickButton3();
private:
#pragma endregion
};
