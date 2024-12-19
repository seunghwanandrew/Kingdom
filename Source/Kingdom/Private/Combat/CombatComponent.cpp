#include "Combat/CombatComponent.h"
#include "GameFramework/Character.h"
#include "Character/PlayerClass.h"
#include "Kismet/KismetMathLibrary.h"

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();
	CharacterRef = Cast<ACharacter>(GetOwner());
}

void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCombatComponent::ComboAttack()
{
	CharacterRef->PlayAnimMontage(AttackAnimations[ComboCounter]);
	
	ComboCounter++;
	int MaxCombo = AttackAnimations.Num() - 1;
	ComboCounter = UKismetMathLibrary::Wrap(ComboCounter, -1, MaxCombo);
}


