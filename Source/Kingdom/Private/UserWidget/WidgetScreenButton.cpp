#include "UserWidget/WidgetScreenButton.h"
#include "Components/SizeBox.h"
#include "Components/Overlay.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UWidgetScreenButton::NativePreConstruct()
{
	DesignSizeMode = EDesignPreviewSizeMode::Desired;
	SizeBoxBase->SetWidthOverride(Width);
	SizeBoxBase->SetHeightOverride(Height);
	TextBlock->SetText(ButtonText);
}

void UWidgetScreenButton::NativeConstruct()
{
	ButtonBase->OnClicked.AddDynamic(this, &UWidgetScreenButton::OnButtonClicked);
}

void UWidgetScreenButton::OnButtonClicked()
{
	OnScreenButtonClickDelegate.Broadcast();
}


void UWidgetScreenButton::SetText(FName Text)
{
	TextBlock->SetText(FText::FromString(Text.ToString()));
}
