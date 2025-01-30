#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"


class LOADINGSCREENMODULE_API SPreLoadingScreen : public SCompoundWidget
{
public:

#pragma region Variables
public:
protected:
private:
	UTexture2D* BackgroundTexture;
	TSharedPtr<FSlateBrush> BackgroundBrush;
#pragma endregion


#pragma region Functions
public:
	SLATE_BEGIN_ARGS(SPreLoadingScreen)
		:_BackgroundTexture(nullptr)
	{}
	SLATE_ARGUMENT(UTexture2D*, BackgroundTexture)

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
protected:
private:
#pragma endregion
};
