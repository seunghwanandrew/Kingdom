#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TargetDisplayer.generated.h"

class UImage;

UCLASS()
class KINGDOM_API UTargetDisplayer : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))
	UImage* TargetDisplayer;
};
