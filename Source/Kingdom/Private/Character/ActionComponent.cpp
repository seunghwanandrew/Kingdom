#include "Character/ActionComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interfaces/PlayerInterface.h"

UActionComponent::UActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UActionComponent::BeginPlay()
{
	Super::BeginPlay();
	CharacterRef = Cast<ACharacter>(GetOwner());
	MoveComponent = CharacterRef->GetCharacterMovement();

	if (CharacterRef->Implements<UPlayerInterface>())
	{
		PlayerInterface = Cast<IPlayerInterface>(CharacterRef);
	}
}

void UActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UActionComponent::BasicWalkMovement()
{
	MoveComponent->MaxWalkSpeed = WalkMoveSpeed;
}

void UActionComponent::StealthMovement()
{
	if (!PlayerInterface->Execute_HasEnoughStamina(CharacterRef, StaminaToUseForStealthMove))
	{
		BasicWalkMovement();
		return;
	}

	if (MoveComponent->Velocity.Equals(FVector::ZeroVector, 1)) { return; }

	MoveComponent->MaxWalkSpeed = StealthMoveSpeed;

	OnStealthMovementDelegate.Broadcast(StaminaToUseForStealthMove);
}

