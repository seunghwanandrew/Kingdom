#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ActionComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(FOnMovementStaminaUsageSignature, UActionComponent, OnMovementStaminaUsageDelegate, float, StaminaUsage);

class IPlayerInterface;
class UCharacterMovementComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class KINGDOM_API UActionComponent : public UActorComponent
{
	GENERATED_BODY()

#pragma region Variables
public:
	UPROPERTY(BlueprintAssignable)
	FOnMovementStaminaUsageSignature OnMovementStaminaUsageDelegate;
protected:
private:
	ACharacter* CharacterRef;
	IPlayerInterface* PlayerInterface;
	UCharacterMovementComponent* MoveComponent;

	UPROPERTY(EditAnywhere, Category = Movement, meta = (AllowPrivateAccess = "ture"))
	float StaminaToUseMovement = 0.04f;
	UPROPERTY(EditAnywhere, Category = Movement, meta = (AllowPrivateAccess = "ture"))
	float StealthMoveSpeed = 100.0f ;
	UPROPERTY(EditAnywhere, Category = Movement, meta = (AllowPrivateAccess = "ture"))
	float AimWalkMoveSpeed = 300.0f;
	UPROPERTY(EditAnywhere, Category = Movement, meta = (AllowPrivateAccess = "ture"))
	float WalkMoveSpeed = 400.0f;
	UPROPERTY(EditAnywhere, Category = Movement, meta = (AllowPrivateAccess = "ture"))
	float SprintMoveSpeed = 600.0f;
	UPROPERTY(EditAnywhere, Category = Movement, meta = (AllowPrivateAccess = "ture"))
	float RollingStaminaUsage = 3.5f;
	UPROPERTY(EditAnywhere, Category = Movement, meta = (AllowPrivateAccess = "ture"))
	bool bIsSprintMode = false;
	UPROPERTY(EditAnywhere, Category = Movement, meta = (AllowPrivateAccess = "ture"))
	bool bIsRollActive = false;

	/* Montage */
	UPROPERTY(EditAnywhere, Category = Montage, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* RollMontage;

#pragma endregion

#pragma region Functions
public:	
	/* Getter */
	FORCEINLINE bool IsSprintMode() { return bIsSprintMode; }
	FORCEINLINE bool IsRollingMode() { return bIsRollActive; }

	/* Setter */
	FORCEINLINE void SetSprintMode(bool Value) { bIsSprintMode = Value; }

	UActionComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void BasicWalkMovement();
	UFUNCTION(BlueprintCallable)
	void StealthMovement();
	UFUNCTION(BlueprintCallable)
	void AimWalkMovement();
	UFUNCTION(BlueprintCallable)
	void SprintMovement();
	UFUNCTION(BlueprintCallable)
	void Roll();
	UFUNCTION(BlueprintCallable)
	void FinishRollAnimation();
protected:
	virtual void BeginPlay() override;

private:
#pragma endregion
		
};
