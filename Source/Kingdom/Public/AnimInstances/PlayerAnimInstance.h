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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	AEquipableItemClass* CurrentEquippedWeapon;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	APlayerClass* PlayerRef;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	FVector Velocity{ 0.0f, 0.0f, 0.0f };
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	float GroundSpeed{ 0.0f };
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	float GroundDirection{ 0.0f };
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	bool bShouldMove;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	bool bIsFalling;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat)
	bool bIsOnTarget;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = State)
	EPlayerActionState ActionState;
private:
#pragma endregion
#pragma region Function
public:
	virtual void NativeUpdateAnimation(float DeltaTime) override;
	void OnUpdateTargetHandle();

	/* Setter */
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetActionState(const EPlayerActionState& StateCondition) { ActionState = StateCondition; }
protected:
	virtual void NativeInitializeAnimation() override;
	UFUNCTION(BlueprintCallable)
	void UnEquipPoint();
	UFUNCTION(BlueprintCallable)
	void UnEquip();
	UFUNCTION(BlueprintCallable)
	void Equip();
	UFUNCTION(BlueprintCallable)
	void AttackEnd();
	UFUNCTION(BlueprintCallable)
	void EndEngaging();
private:
	bool IsShouldMove();
	bool IsFalling();
	float SetGroundSpeed();
	float SetGroundDirection();

#pragma endregion
};
