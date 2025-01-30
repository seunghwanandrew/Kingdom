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
	UNiagaraComponent* NiagaraEffect;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "ture"))
	UParticleSystemComponent* ParticleEffect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "ture"))
	UParticleSystem* HitEffect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "ture"))
	USoundBase* GetArrowHitSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "ture"))
	UProjectileMovementComponent* ProjectileMovement;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	USphereComponent* SphereCollision;
	UPROPERTY(EditAnywhere, Category = Projectile, meta = (AllowPrivateAccess = "true"))
	float GravityScale = 1.0f;
	UPROPERTY(EditAnywhere, Category = Projectile, meta = (AllowPrivateAccess = "true"))
	float MultipleArrowPower = 2.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile, meta = (AllowPrivateAccess = "true"), meta = (ExposeOnSpawn = "true"))
	float ProjectileSpeed;
#pragma endregion
#pragma region Functions
public:
	AProjectile();
	virtual void Tick(float DeltaTime) override;
	void AddDelegate();

	UFUNCTION(BlueprintAuthorityOnly)
	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void SetVelocity(FVector Vector);
	void SetZeroGravity();
	void SetProjectileSpeed_Gravity(float InitSpeed, float MaxSpeed, float GravityValue);
protected:
	virtual void BeginPlay() override;
private:
#pragma endregion
};
