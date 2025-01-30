#pragma once

#include "CoreMinimal.h"
#include "Items/BaseItemClass.h"
#include "PickableItemClass.generated.h"

class UNiagaraSystem;

UCLASS()
class KINGDOM_API APickableItemClass : public ABaseItemClass
{
	GENERATED_BODY()
	
#pragma region Variables
public:
protected:
	UPROPERTY(EditAnywhere, Category = ItemValue)
	int32 Amount = 0;
	UPROPERTY(EditAnywhere, Category = ItemValue)
	int32 Soul = 0;

	UPROPERTY(EditAnywhere, Category = Effect)
	UNiagaraSystem* PickupEffect;
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Location, meta = (AllowPrivateAccess = "true"))
	FVector TargetHeight = FVector{ 0.0f, 0.0f, 80.0f };
	UPROPERTY(VisibleAnywhere, Category = Location)
	float DesiredTargetLocationZValue;

	UPROPERTY(EditAnywhere, Category = Debug)
	bool bIsDebugMode = false;
#pragma endregion

#pragma region Functions
public:
	/* Setter */
	FORCEINLINE void SetSoulValue(int32 Value) { Soul = Value; }

	APickableItemClass();
	virtual void Tick(float DeltaTime) override;
	void ItemSpawningDown(float DeltaTime);
protected:
	virtual void BeginPlay() override;

	void GetObjectSurfaceZLocation();

	virtual void HandleOverlappedItemEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
private:
#pragma endregion
};
