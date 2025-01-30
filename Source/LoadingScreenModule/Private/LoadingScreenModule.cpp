#include "LoadingScreenModule.h"
#include "PreLoadingScreen/SPreLoadingScreen.h"
#include "MoviePlayer.h"

void FLoadingScreenModule::StartupModule()
{
	UE_LOG(LogTemp, Warning, TEXT("FLoadingScreenModule::StartupModule"));
	
	BackgroundTexture = LoadObject<UTexture2D>(nullptr, TEXT("Texture2D'/Game/5_UserInterFace/Asset/LoadScreen/LoadScreenBG.LoadScreenBG'"));
}

bool FLoadingScreenModule::IsGameModule() const
{
	return false;
}

void FLoadingScreenModule::StartupLoadingScreen()
{
	UE_LOG(LogTemp, Warning, TEXT("FLoadingScreenModule::StartupLoadingScreen"));
	FLoadingScreenAttributes LoadingScreen;
	LoadingScreen.bAutoCompleteWhenLoadingCompletes = false;
	LoadingScreen.MinimumLoadingScreenDisplayTime = 2.0f;
	LoadingScreen.WidgetLoadingScreen = SNew(SPreLoadingScreen).BackgroundTexture(BackgroundTexture);

	GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);
}

IMPLEMENT_GAME_MODULE(FLoadingScreenModule, LoadingScreenModule);