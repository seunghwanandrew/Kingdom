#pragma once

#include "CoreMinimal.h"
#include "Items/BaseItemClass.h"
#include "EquipableItemClass.generated.h"

class UTraceComponent;

UCLASS()
class KINGDOM_API AEquipableItemClass : public ABaseItemClass
{
	GENERATED_BODY()
#pragma region Variables
public:
protected:
	/* Combat Component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"));
	UTraceComponent* Trace;
private:
#pragma endregion

#pragma region Functions
public:
	/* Getter */
	FORCEINLINE UTraceComponent* GetTraceComponent() { return Trace; }

	AEquipableItemClass();
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable)
	virtual void AttachMeshtoSocket(USceneComponent* Parent, const FName& SocketName);
	void Set_EmptyTraceTargetToIgnoreVariable();
protected:
	virtual void BeginPlay() override;

	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	virtual void HandleOverlappedItemEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void HandleEndOverlappedItemEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
private:
#pragma endregion
};