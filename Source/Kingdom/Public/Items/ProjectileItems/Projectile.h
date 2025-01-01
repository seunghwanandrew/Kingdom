#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class UNiagaraComponent;
class UProjectileMovementComponent;
class USphereComponent;

UCLASS()
class KINGDOM_API AProjectile : public AActor
{
	GENERATED_BODY()
	
#pragma region Variables
public:
protected:
private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "ture"))
	USceneComponent* DefaultSceneRoot;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "ture"))
	UStaticMeshComponent* Projectile;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "ture"))
	UNiagaraComponent* Effect;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "ture"))
	UProjectileMovementComponent* ProjectileMovement;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "ture"))
	USphereComponent* SphereCollision;
#pragma endregion
#pragma region Functions
public:	
	AProjectile();
	virtual void Tick(float DeltaTime) override;

	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
protected:
	virtual void BeginPlay() override;
private:
#pragma endregion
};
