#include "PreLoadingScreen/SPreLoadingScreen.h"
#include "SlateOptMacros.h"
#include "SlateExtras.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SPreLoadingScreen::Construct(const FArguments& InArgs)
{
	UE_LOG(LogTemp, Warning, TEXT("Slate Construct"));

	BackgroundTexture = InArgs._BackgroundTexture;

	BackgroundBrush = MakeShareable(new FSlateBrush());
	BackgroundBrush->SetResourceObject(BackgroundTexture);

	ChildSlot
		[
			SNew(SOverlay)
				+ SOverlay::Slot()
				.HAlign(EHorizontalAlignment::HAlign_Fill)
				.VAlign(EVerticalAlignment::VAlign_Fill)
				[
					SNew(SImage)
						.Image(BackgroundTexture ? BackgroundBrush.Get() : nullptr)
						.ColorAndOpacity(FLinearColor::White)
				]
				+ SOverlay::Slot()
				.HAlign(EHorizontalAlignment::HAlign_Right)
				.VAlign(EVerticalAlignment::VAlign_Bottom)
				.Padding(10.0f)
				[
					SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
						.AutoWidth()
						[
							SNew(SThrobber)
								.NumPieces(10)
						]
						+ SHorizontalBox::Slot()
						.AutoWidth()
						.VAlign(EVerticalAlignment::VAlign_Center)
						.Padding(FMargin(10.0f, 0.0f, 0.0f, 0.0f))
						[
							SNew(STextBlock)
								.Text(FText::FromString(TEXT("Loading")))
								.Font(FSlateFontInfo(FPaths::ProjectContentDir()/TEXT("/5_UserInterFace/Asset/Font/CinzelDecorative-Black.ttf"), 24))
								.ColorAndOpacity(FLinearColor::White)
						]
				]
		];
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION
