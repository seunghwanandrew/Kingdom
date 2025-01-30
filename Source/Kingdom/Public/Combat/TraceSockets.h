#pragma once

#include "CoreMinimal.h"
#include "TraceSockets.generated.h"

USTRUCT(BlueprintType)
struct KINGDOM_API FTraceSockets
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat)
	FName Start;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat)
	FName End;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat)
	FName Rotation;
};
