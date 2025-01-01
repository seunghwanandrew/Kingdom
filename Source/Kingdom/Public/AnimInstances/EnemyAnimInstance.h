#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimSequenceBase.h"
#include "EnumClass/EnumClass.h"
#include "EnemyAnimInstance.generated.h"

UCLASS()
class KINGDOM_API UEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
#pragma region Variables
public:
	UPROPERTY(BlueprintReadWrite, Category = State)
	bool bIsDead;
	UPROPERTY(BlueprintReadWrite, Category = "Animation")
	UAnimSequenceBase* DeathPose1;
	UPROPERTY(BlueprintReadWrite, Category = "Animation")
	UAnimSequenceBase* DeathPose2;
protected:
	UPROPERTY(BlueprintReadWrite, Category = Character)
	ACharacter* EnemyRef;
	UPROPERTY(BlueprintReadWrite, Category = State)
	EDeathPose DeathPose;

private:
	UPROPERTY(BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	FVector Velocity;
	UPROPERTY(BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float GroundSpeed;
	UPROPERTY(BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float LookRotation;
#pragma endregion

#pragma region Functions
public:
	virtual void NativeUpdateAnimation(float DeltaTime) override;
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetDeathPose(const EDeathPose& StateCondition) { DeathPose = StateCondition; }
protected:
	virtual void NativeInitializeAnimation() override;
private:
	void SetGroundSpeed();
	void SetLookRotation();

#pragma endregion
};
