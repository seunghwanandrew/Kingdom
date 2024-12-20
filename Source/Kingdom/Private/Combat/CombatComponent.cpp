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
	IPlayerInterface* PlayerInterface = nullptr;
	if (CharacterRef->Implements<UPlayerInterface>())
	{
		PlayerInterface = Cast<IPlayerInterface>(CharacterRef);
	}

	if (!PlayerInterface->Execute_HasEnoughStamina(CharacterRef, StaminaToUseForAttacking))
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Not Enought Stamina is remained"));
		return;
	}
	CharacterRef->PlayAnimMontage(AttackAnimations[ComboCounter]);
	
	ComboCounter++;
	int MaxCombo = AttackAnimations.Num() - 1;
	ComboCounter = UKismetMathLibrary::Wrap(ComboCounter, -1, MaxCombo);

	OnAttackPerformedDelegate.Broadcast(StaminaToUseForAttacking);
}


