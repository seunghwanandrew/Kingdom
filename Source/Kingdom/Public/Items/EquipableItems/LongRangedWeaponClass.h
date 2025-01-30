#pragma once

#include "CoreMinimal.h"
#include "Items/EquipableItems/EquipableItemClass.h"
#include "LongRangedWeaponClass.generated.h"

UCLASS()
class KINGDOM_API ALongRangedWeaponClass : public AEquipableItemClass
{
	GENERATED_BODY()
#pragma region Variables
public:
protected:
private:
#pragma endregion

#pragma region Functions
public:
	ALongRangedWeaponClass();
	virtual void Tick(float DeltaTime) override;
protected:
	virtual void BeginPlay() override;
private:
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	virtual void HandleOverlappedItemEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void HandleEndOverlappedItemEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
#pragma endregion
};
