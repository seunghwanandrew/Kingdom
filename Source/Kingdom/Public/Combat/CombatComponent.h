#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(FOnAttackPerformedSignature, UCombatComponent, OnAttackPerformedDelegate, float, AttackStamina);

class UAnimMontage;

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
private:
	/* Montage */
	UPROPERTY(EditAnywhere, Category = Montage)
	TArray<UAnimMontage*> AttackAnimations;
	UPROPERTY(VisibleAnywhere, Category = Attacking)
	int ComboCounter = 0;
	UPROPERTY(EditAnywhere, Category = Attacking)
	float StaminaToUseForAttacking = 5.0f;

#pragma endregion

#pragma region Functions
public:	
	UCombatComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void ComboAttack();
protected:
	virtual void BeginPlay() override;
private:
#pragma endregion		
};
