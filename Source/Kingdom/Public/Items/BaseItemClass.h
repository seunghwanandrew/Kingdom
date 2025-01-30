#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnumClass/EnumClass.h"
#include "NiagaraComponent.h"
#include "BaseItemClass.generated.h"

class USkeletalMeshComponent;
class UStaticMeshComponent;
class UCapsuleComponent;
class USphereComponent;
class UNiagaraComponent;
class UNiagaraSystem;

UCLASS()
class KINGDOM_API ABaseItemClass : public AActor
{
	GENERATED_BODY()
#pragma region Variables
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	USkeletalMeshComponent* SkeletalMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	UStaticMeshComponent* StaticMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	UCapsuleComponent* CapsuleCollisionComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	USphereComponent* SphereCollisionComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	UNiagaraComponent* NiagaraComponent;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item State")
	EItemState ItemState = EItemState::EIS_None;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TrigData", meta= (AllowPrivateAccess = "true"))
	float RunningTime{ 0.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TrigData", meta = (AllowPrivateAccess = "true"))
	float Amplitude{ 0.3f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TrigData", meta = (AllowPrivateAccess = "true"))
	float TimeConstant{ 3.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TrigData", meta = (AllowPrivateAccess = "true"))
	float RotationSpeed{ 90.0f };

#pragma endregion 

#pragma region Function
public:	
	ABaseItemClass();
	virtual void Tick(float DeltaTime) override;

	void SetSphereColliderCollision(ECollisionEnabled::Type Condition);

	/* Setter */
	FORCEINLINE void SetItemState(EItemState Condition) { ItemState = Condition; }
	FORCEINLINE void DisableNiagaraComponent() { if (IsValid(NiagaraComponent)) NiagaraComponent->SetVisibility(false); }
	FORCEINLINE void AbleNiagraComponent() { if(IsValid(NiagaraComponent)) NiagaraComponent->SetVisibility(true); }

protected:
	virtual void BeginPlay() override;
	UFUNCTION(BlueprintAuthorityOnly)
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION(BlueprintAuthorityOnly)
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
	virtual void TransformedPosition(float DeltaTime);
	UFUNCTION(BlueprintCallable)
	virtual void TransformedRotator(float DeltaTime);

	UFUNCTION(BlueprintCallable)
	void CreateSkeletalMeshComponentAsRootComponent();
	UFUNCTION(BlueprintCallable)
	void CreateStaticMeshComponentAsRootComponent();
	UFUNCTION(BlueprintCallable)
	void CreateCapsuleCollisionComponentAsRootComponent();
	UFUNCTION(BlueprintCallable)
	void CreateSphereCollisionComponentAsRootComponent();
	UFUNCTION(BlueprintCallable)
	void CreateSphereCollisionComponent();
	UFUNCTION(BlueprintCallable)
	void CreateNiagaraComponent();

	virtual void HandleOverlappedItemEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	virtual void HandleEndOverlappedItemEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
private:

#pragma endregion 

};
