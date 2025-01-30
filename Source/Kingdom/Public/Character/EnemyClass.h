// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterClass.h"
#include "Interfaces/EnemyInterface.h"
#include "Interfaces/HitInterface.h"
#include "UserWidget/EnemyHealthBarComponent.h"
#include "UserWidget/EnemyHealthBar.h"
#include "EnumClass/EnumClass.h"
#include "EnemyClass.generated.h"

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_TwoParams(FOnTargetDetectedSignature, AEnemyClass, OnTargetDectectedDelegate, AActor*, Target, bool, bIsTarget);
DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE(FDeathNotifySignature, AEnemyClass, OnDeathNotifyDelegate);

class AAIController;
class AEquipableItemClass;
class ACloseRangedWeaponClass;
class UBlackboardComponent;
class UEnemyHealthBarComponent;
class UEnemyAnimInstance;
class UMotionWarpingComponent;
class UObject;
class UPawnSensingComponent;
class USphereComponent;
class UTargetDisplayerWidgetComponent;
class UTraceComponent;
class APickableItemClass;

struct FTimerHandle;

UCLASS()
class KINGDOM_API AEnemyClass : public ACharacterClass, public IEnemyInterface
{
	GENERATED_BODY()

#pragma region Variables
public:
	UPROPERTY(BlueprintAuthorityOnly)
	FOnTargetDetectedSignature OnTargetDectectedDelegate;
	UPROPERTY(BlueprintAuthorityOnly)
	FDeathNotifySignature OnDeathNotifyDelegate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EnemyState)
	UBlackboardComponent* Blackboard;

protected:
	UPROPERTY(VisibleAnywhere, Category = Controller)
	AAIController* EnemyController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RandomSpawnRate;
	UPROPERTY(EditAnywhere, Category = SpawnItem)
	TArray<TSubclassOf<APickableItemClass>> PossibleDropItem;

	/* Monatage */
	UPROPERTY(EditAnywhere, Category = Montage)
	TArray<UAnimMontage*> TransformMotionList;
	UPROPERTY(EditAnywhere, Category = Montage)
	UAnimMontage* MontageToTransformPlay;

	/* Combat Component */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat)
	UPawnSensingComponent* PawnSensing;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat)
	UTraceComponent* TraceComponent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat)
	UMotionWarpingComponent* MotionWarping;

	/* Enemy Widget Component */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat)
	UEnemyHealthBarComponent* HealthBarComponent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat)
	float InitWalkSpeed = 55.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat)
	float MaxWalkSpeed = 550.0f;

	/* Movement Variable */
	UPROPERTY(EditAnywhere, Category = "AI Movement")
	TArray<AActor*> RoamingTargetDestination;

	/* Combat Variable */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat)
	AActor* DetectedTarget;
	float WarpTargetDistance = 25.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat)
	bool bIsDead = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat)
	TSubclassOf<AEquipableItemClass> Weapon;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat)
	int32 Souls;
	AEquipableItemClass* DefaultWeapon;

	/* Anim Instance */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat);
	UEnemyAnimInstance* AnimInstance;
private:
	
#pragma endregion

#pragma region Functions
public:
	/* Setter */
	FORCEINLINE void SetDetectedTarget(AActor* Target) { DetectedTarget = Target; }
	FORCEINLINE void SetHealthBarVisibility(bool Value) { HealthBarComponent->SetVisibility(Value); }
	/* Getter */
	FORCEINLINE UTraceComponent* GetTraceComponent() { return TraceComponent; }
	UTraceComponent* GetCurrentEquipWeaponTraceComponent();
	FORCEINLINE float GetInitWalkSpeedValue() { return InitWalkSpeed; }
	FORCEINLINE float GetMaxWalkSpeedValue() { return MaxWalkSpeed; }
	FORCEINLINE bool HasDefaultWeapon() { return (DefaultWeapon != nullptr) ? true : false; }

	AEnemyClass();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void Attack();
	void HandleAttackEnd();
	FVector RandomDestinationLocation();
	UAnimMontage* GetTransformPlayMontage();
	void SetTransfromPlayMontage(UAnimMontage* MontageToPlay);
	void SelectRandomTransformPlayMontage();
	
	/* Interfaces */
	virtual void GetHit_Implementation(float DamageAmount, AController* EventInstigator, AActor* DamageCauser, FVector ImpactPoint) override;
	virtual bool IsBoss_Implementation() override;

	/* Event Handler */
	UFUNCTION(BlueprintAuthorityOnly)
	void OnSeePawn(APawn* Pawn);
	UFUNCTION(BlueprintAuthorityOnly)
	void OnDeath(int32 Index);

	/* Custom Event */
	void SetWarpTargetTranslation();

protected:
	virtual void BeginPlay() override;

private:
	virtual void Initialize() override;
	virtual void CreateInitComponent() override;
	virtual void CreateCombatComponent() override;
	bool InRange(AActor* Target, float AcceptableRadius);
#pragma endregion
};
