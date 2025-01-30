#include "GameInstance/KingdomGameInstance.h"
#include "LoadingScreenModule.h"

void UKingdomGameInstance::Init()
{
	Super::Init();

	FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &UKingdomGameInstance::OnPreLoadMap);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UKingdomGameInstance::OnDestinationWorldMapLoaded);
}

void UKingdomGameInstance::OnPreLoadMap(const FString& MapName)
{
	FLoadingScreenModule* LoadingScreenModule = FModuleManager::LoadModulePtr<FLoadingScreenModule>("LoadingScreenModule");
	if (LoadingScreenModule == nullptr) return;
	LoadingScreenModule->StartupLoadingScreen();
}

void UKingdomGameInstance::OnDestinationWorldMapLoaded(UWorld* LoadedWorld)
{
}
