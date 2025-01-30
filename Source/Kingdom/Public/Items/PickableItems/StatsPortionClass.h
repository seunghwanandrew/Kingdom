#pragma once

#include "CoreMinimal.h"
#include "Items/PickableItems/PickableItemClass.h"
#include "StatsPortionClass.generated.h"


UCLASS()
class KINGDOM_API AStatsPortionClass : public APickableItemClass
{
	GENERATED_BODY()

#pragma region Variables
public:
	UPROPERTY(EditAnywhere, Category = Stats)
	float HealthRechargeAmount;
protected:
private:
#pragma endregion

#pragma region Functions
public:
	AStatsPortionClass();
	virtual void Tick(float DeltaTime) override;
protected:
	virtual void BeginPlay() override;

	virtual void HandleOverlappedItemEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
private:
};
