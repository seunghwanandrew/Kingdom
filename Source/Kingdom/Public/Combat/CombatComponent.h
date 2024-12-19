#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"

class UAnimMontage;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class KINGDOM_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

#pragma region Variable
public:
protected:
	ACharacter* CharacterRef;
private:
	/* Montage */
	UPROPERTY(EditAnywhere, Category = Montage)
	TArray<UAnimMontage*> AttackAnimations;
	UPROPERTY(VisibleAnywhere, Category = Montage)
	int ComboCounter = 0;
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
