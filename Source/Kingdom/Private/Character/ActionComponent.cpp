#include "Character/ActionComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interfaces/PlayerInterface.h"
#include "Kismet/KismetMathLibrary.h"

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

	if (bIsSprintMode)
	{
		if (MoveComponent->Velocity.Equals(FVector::ZeroVector, 1)) { return; }
		OnMovementStaminaUsageDelegate.Broadcast(StaminaToUseMovement);
	}
}

void UActionComponent::BasicWalkMovement()
{
	MoveComponent->MaxWalkSpeed = WalkMoveSpeed;
}

void UActionComponent::StealthMovement()
{
	if (!PlayerInterface->Execute_HasEnoughStamina(CharacterRef, StaminaToUseMovement))
	{
		BasicWalkMovement();
		return;
	}

	if (MoveComponent->Velocity.Equals(FVector::ZeroVector, 1)) { return; }

	MoveComponent->MaxWalkSpeed = StealthMoveSpeed;

	OnMovementStaminaUsageDelegate.Broadcast(StaminaToUseMovement);
}

void UActionComponent::AimWalkMovement()
{
	MoveComponent->MaxWalkSpeed = AimWalkMoveSpeed;
}

void UActionComponent::SprintMovement()
{
	if (!PlayerInterface->Execute_HasEnoughStamina(CharacterRef, StaminaToUseMovement))
	{
		BasicWalkMovement();
		bIsSprintMode = false;
		return;
	}

	bIsSprintMode = true;
	MoveComponent->MaxWalkSpeed = SprintMoveSpeed;
}

void UActionComponent::Roll()
{
	if (bIsRollActive || !PlayerInterface->Execute_HasEnoughStamina(CharacterRef, RollingStaminaUsage))	return;

	bIsRollActive = true;

	OnMovementStaminaUsageDelegate.Broadcast(RollingStaminaUsage);

	FVector Direction = CharacterRef->GetCharacterMovement()->Velocity.Length() < 1 ? CharacterRef->GetActorForwardVector() : CharacterRef->GetLastMovementInputVector();
	FRotator NewRotator = UKismetMathLibrary::MakeRotFromX(Direction);
	CharacterRef->SetActorRotation(NewRotator);
	CharacterRef->PlayAnimMontage(RollMontage);
	float PlayTime = RollMontage->GetPlayLength();

	FTimerHandle RollTimerHandle;

	GetWorld()->GetTimerManager().SetTimer(RollTimerHandle, this, &UActionComponent::FinishRollAnimation, PlayTime);
}

void UActionComponent::FinishRollAnimation()
{
	bIsRollActive = false;
}