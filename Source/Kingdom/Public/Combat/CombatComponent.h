#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(FOnAttackPerformedSignature, UCombatComponent, OnAttackPerformedDelegate, float, AttackStamina);

class UAnimMontage;
class AProjectile;
class USplineComponent;
class USplineMeshComponent;
struct FPredictProjectilePathResult;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class KINGDOM_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

#pragma region Variables
public:
	UPROPERTY(BlueprintAssignable)
	FOnAttackPerformedSignature OnAttackPerformedDelegate;

protected:
	ACharacter* CharacterRef;

	UPROPERTY(EditAnywhere, Category = Projectile)
	TSubclassOf<AProjectile> ArrowBlueprint;
	UPROPERTY(EditAnywhere, Category = Projectile)
	int32 InitArrowHold = 30;
	UPROPERTY(VisibleAnywhere, Category = Projectile)
	int32 Soul = 0;
	UPROPERTY(EditAnywhere, Category = Projectile)
	TArray<AProjectile*> Arrows;
	UPROPERTY(EditAnywhere, Category = Projectile)
	AActor* ArrowSplines;
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	USplineComponent* ArrowPath;
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	USplineMeshComponent* ProjectilePathComponent;
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	UStaticMeshComponent* ArcEnd;
	UPROPERTY(EditAnywhere, Category = Projectile)
	UStaticMesh* PathComponent;
	UPROPERTY(VisibleInstanceOnly, Category = Projectile)
	TArray<USplineMeshComponent*> SplineMeshes;

private:
	/* Montage */
	UPROPERTY(EditAnywhere, Category = Montage)
	TArray<UAnimMontage*> AttackMontageList;
	UPROPERTY(EditAnywhere, Category = Montage)
	UAnimMontage* AttackMontage;
	UPROPERTY(EditAnywhere, Category = Montage)
	UAnimMontage* BowFireMontage;
	UPROPERTY(VisibleAnywhere, Category = Attacking)
	int ComboCounter = 0;
	UPROPERTY(EditAnywhere, Category = Attacking)
	float StaminaToUseForAttacking = 2.0f;
	UPROPERTY(EditAnywhere, Category = Projectile)
	float ArrowSpeed = 6500.0f;
	UPROPERTY(EditAnywhere, Category = Projectile)
	float Gravity = 1.0f;	
	UPROPERTY(EditAnywhere, Category = Debug)
	bool bIsProjectileDebugLine = false;

	FHitResult OutHitResult;
	FVector TargetArrowSpawnLocation;
	FRotator TargetArrowSpawnRotation;
	FVector ArrowSpawnLocation;
	FRotator ArrowSpawnRotation;
	FTransform SpawnTransform;
	FVector ImpactPoint;
	FVector FinalArcLocation;
#pragma endregion

#pragma region Functions
public:	
	/* Getter */
	FORCEINLINE int32 GetCurrentHoldArrowAmount() { return Arrows.Num(); }
	FORCEINLINE int32 GetCurrentSoulAmount() { return Soul; }

	/* Setter */
	FORCEINLINE void SetCurrentSoulAmount(int32 Amount) { Soul += Amount; }

	UCombatComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void DrawProjectilePath();
	void ClearProjectilePath();
	FPredictProjectilePathResult ProjectilePath();
	void SetSplineMesh(FPredictProjectilePathResult& PathResult);
	void Player_ComboAttack();
	void Player_ProjectileEjection();
	void PlayArrowFireMontage();
	void SpawnArrow();
	void Enemy_Attack();
	void SetCurrentHoldArrowAmount(int32 Amount);
	AProjectile* GetArrowFromPool();
protected:
	virtual void BeginPlay() override;
private:
	void LineTrace(float DeltaTime);
	void CreateArrows(int32 Amount);
#pragma endregion		
};
