#pragma once

#include "CoreMinimal.h"
#include "Character/EnemyClass.h"
#include "BossEnemyClass.generated.h"

class UTargetDisplayerWidgetComponent;

UCLASS()
class KINGDOM_API ABossEnemyClass : public AEnemyClass
{
	GENERATED_BODY()
	
#pragma region Variables
public:
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat)
	UTargetDisplayerWidgetComponent* TargetDisplayer;
private:
#pragma endregion

#pragma region Functions
public:
	ABossEnemyClass();
	virtual void Tick(float DeltaTime) override;
protected:
	virtual void BeginPlay() override;

	void OnTargeted_Implementation() override;
	void UnTargeted_Implementation() override;
private:
#pragma endregion
};
