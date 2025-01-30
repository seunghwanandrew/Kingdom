#include "UserWidget/GameMenuScreen.h"
#include "UserWidget/WidgetScreenButton.h"
#include "Components/TextBlock.h"

void UGameMenuScreen::NativePreConstruct()
{
}

void UGameMenuScreen::NativeConstruct()
{
	MainText->SetText(FText::FromString(MainTextName.ToString()));
	if (SubTitleName == FName("None"))
	{
		SubTitle->SetText(FText::GetEmpty());
	}
	else
	{
		SubTitle->SetText(FText::FromString(SubTitleName.ToString()));
	}
	Button1->SetText(Button1Name);
	Button2->SetText(Button2Name);
	Button3->SetText(Button3Name);
	Button1->OnScreenButtonClickDelegate.AddDynamic(this, &UGameMenuScreen::OnClickButton1);
	Button2->OnScreenButtonClickDelegate.AddDynamic(this, &UGameMenuScreen::OnClickButton2);
	Button3->OnScreenButtonClickDelegate.AddDynamic(this, &UGameMenuScreen::OnClickButton3);

	if (AnimationToPlay)
	{
		PlayAnimation(AnimationToPlay);
	}
}

