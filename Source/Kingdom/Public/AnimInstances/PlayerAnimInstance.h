#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnumClass/EnumClass.h"
#include "PlayerAnimInstance.generated.h"

class APlayerClass;
class AEquipableItemClass;

UCLASS()
class KINGDOM_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
#pragma region Variable
public:
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	APlayerClass* PlayerRef;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	FVector VelocityValue{ 0.0f, 0.0f, 0.0f };
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	float GroundSpeed{ 0.0f };
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	float GroundDirection{ 0.0f };
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	float YawOffset{ 0.0f };
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	float PitchOffset{ 0.0f };
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	bool bShouldMove;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	bool bIsFalling;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat)
	bool bIsOnTarget;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat)
	bool bIsAiming;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = State)
	EPlayerActionState ActionState = EPlayerActionState::EPAS_None;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = State)
	EDeathPose DeathPose;
private:
#pragma endregion
#pragma region Function
public:
	/* Setter */
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetActionState(const EPlayerActionState& StateCondition) { ActionState = StateCondition; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetDeathPose(const EDeathPose& StateCondition) { DeathPose = StateCondition; }
	FORCEINLINE void SetIsAiming(bool Value) { bIsAiming = Value; }

	virtual void NativeUpdateAnimation(float DeltaTime) override;
	void OnUpdateTargetHandle();

protected:
	virtual void NativeInitializeAnimation() override;

private:
	bool IsShouldMove();
	bool IsFalling();
	float SetGroundSpeed();
	float SetGroundDirection();
	float GetYawOffset();
	float GetPitchOffset();
	float GetRootYawOffset(float DeltaTime);
#pragma endregion
};
