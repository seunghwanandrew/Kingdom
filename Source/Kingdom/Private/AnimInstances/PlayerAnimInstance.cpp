#include "AnimInstances/PlayerAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/PlayerClass.h"

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	bShouldMove = IsShouldMove();
	bIsFalling = IsFalling();
	GroundSpeed = SetGroundSpeed();
	GroundDirection = SetGroundDirection();
}

void UPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	PlayerRef = Cast<APlayerClass>(TryGetPawnOwner());
}

void UPlayerAnimInstance::UnEquipPoint()
{
	PlayerRef->UnEquipPoint();
}

void UPlayerAnimInstance::UnEquip()
{
	if (!IsValid(PlayerRef)) return;
	ActionState = EPlayerActionState::EPAS_None;
	PlayerRef->SetActionState(ActionState);
}

void UPlayerAnimInstance::Equip()
{
	if (!IsValid(PlayerRef)) return;
	ActionState = EPlayerActionState::EPAS_Combat;
	PlayerRef->SetActionState(ActionState);
}

void UPlayerAnimInstance::AttackEnd()
{
	if (!IsValid(PlayerRef)) return;
	PlayerRef->HandleAttackEnd();
}

void UPlayerAnimInstance::EndEngaging()
{
	if (!IsValid(PlayerRef)) return;
	PlayerRef->SetIsEngaging(false);
}

void UPlayerAnimInstance::OnUpdateTargetHandle()
{
	if (!IsValid(PlayerRef)) return;
	ActionState = PlayerRef->GetActionState();
}

bool UPlayerAnimInstance::IsShouldMove()
{
	if (!IsValid(PlayerRef)) return false;
	UCharacterMovementComponent* MoveComponent = PlayerRef->GetMoveComponent();
	FVector CurrentAcceleration = MoveComponent->GetCurrentAcceleration();
	return (CurrentAcceleration != FVector::ZeroVector && GroundSpeed > 3.0f);
}

bool UPlayerAnimInstance::IsFalling()
{
	if (!IsValid(PlayerRef)) return false;
	UCharacterMovementComponent* MoveComponent = PlayerRef->GetMoveComponent();
	return MoveComponent->IsFalling();
}

float UPlayerAnimInstance::SetGroundSpeed()
{
	if (!IsValid(TryGetPawnOwner())) return 0.0f;
	Velocity = TryGetPawnOwner()->GetVelocity();
	return static_cast<float>(Velocity.Length());
}

float UPlayerAnimInstance::SetGroundDirection()
{
	if (!IsValid(TryGetPawnOwner())) return 0.0f;
	Velocity = TryGetPawnOwner()->GetVelocity();
	FRotator Rotation = TryGetPawnOwner()->GetActorRotation();
	return CalculateDirection(Velocity, Rotation);
}