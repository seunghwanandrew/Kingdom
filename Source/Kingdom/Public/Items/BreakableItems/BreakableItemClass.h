#pragma once

#include "CoreMinimal.h"
#include "Items/BaseItemClass.h"
#include "Interfaces/HitInterface.h"
#include "Field/FieldSystemTypes.h"
#include "BreakableItemClass.generated.h"

class UGeometryCollectionComponent;
class UFieldSystemComponent;
class UFieldSystemMetaDataFilter;
class URadialFalloff;
class URadialVector;
class APickableItemClass;

struct FChaosBreakEvent;

UCLASS()
class KINGDOM_API ABreakableItemClass : public ABaseItemClass, public IHitInterface
{
	GENERATED_BODY()

#pragma region Variables
public:
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UGeometryCollectionComponent* GeometryComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<APickableItemClass>> PossibleDropItem;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RandomSpawnRate;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = BreakField, meta = (AllowPrivateAccess = "true"))
	UFieldSystemComponent* FieldSystem;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = BreakField, meta = (AllowPrivateAccess = "true"))
	UFieldSystemMetaDataFilter* MetaData;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = BreakField, meta = (AllowPrivateAccess = "true"))
	URadialFalloff* RadialFalloff;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = BreakField, meta = (AllowPrivateAccess = "true"))
	URadialVector* RadialVector;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = BreakField, meta = (AllowPrivateAccess = "true"))
	float MagnitudeValue = 1000000.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = BreakField, meta = (AllowPrivateAccess = "true"))
	float MinRangeValue = 0.8f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = BreakField, meta = (AllowPrivateAccess = "true"))
	float MaxRangeValue = 1.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = BreakField, meta = (AllowPrivateAccess = "true"))
	float DefaultValue = 0.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = BreakField, meta = (AllowPrivateAccess = "true"))
	float RadiusValue = 200.0f;
	EFieldFalloffType FalloffType = EFieldFalloffType::Field_FallOff_None;

#pragma endregion

#pragma region Functions
public:
	ABreakableItemClass();
	virtual void Tick(float DeltaTime) override;

	/* Interface */
	virtual void GetHit_Implementation(float DamageAmount, AController* EventInstigator, AActor* DamageCauser, FVector ImpactPoint) override;
protected:
	virtual void BeginPlay() override;

	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	UFUNCTION(BlueprintNativeEvent)
	void CreateField(const FVector& FieldLocation);
	void DestroyField();
	UFUNCTION(BlueprintAuthorityOnly)
	void OnBreak();
	void SpawnPickableItem(int32 RandomIndex, FTransform& SpawnTransform);
private:
#pragma endregion
};
