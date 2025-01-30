#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "KingdomGameInstance.generated.h"

UCLASS()
class KINGDOM_API UKingdomGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
#pragma region Variables
public:
protected:
private:
#pragma endregion

#pragma region Functions
public:
	virtual void Init() override;
	virtual void OnPreLoadMap(const FString& MapName);
	virtual void OnDestinationWorldMapLoaded(UWorld* LoadedWorld);
protected:
private:
#pragma endregion
};
