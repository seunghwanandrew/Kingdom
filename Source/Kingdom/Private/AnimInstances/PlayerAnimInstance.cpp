#include "AnimInstances/PlayerAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/PlayerClass.h"
#include "Kismet/KismetMathLibrary.h"

void UPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	PlayerRef = Cast<APlayerClass>(TryGetPawnOwner());
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	bShouldMove = IsShouldMove();
	bIsFalling = IsFalling();
	GroundSpeed = SetGroundSpeed();
	GroundDirection = SetGroundDirection();
	YawOffset = GetYawOffset();
	PitchOffset = GetPitchOffset();
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
	VelocityValue = PlayerRef->GetMoveComponent()->Velocity;
	return static_cast<float>(VelocityValue.Length());
}

float UPlayerAnimInstance::SetGroundDirection()
{
	if (!IsValid(TryGetPawnOwner())) return 0.0f;
	VelocityValue = PlayerRef->GetMoveComponent()->Velocity;
	FRotator Rotation = TryGetPawnOwner()->GetActorRotation();
	return CalculateDirection(VelocityValue, Rotation);
}

float UPlayerAnimInstance::GetYawOffset()
{
	if (!IsValid(PlayerRef)) return 0.0f;
	FRotator AimRotation = PlayerRef->GetBaseAimRotation();
	VelocityValue = PlayerRef->GetCharacterMovement()->Velocity;
	FRotator DirectRotation = UKismetMathLibrary::MakeRotFromX(VelocityValue);

	FRotator Delta = UKismetMathLibrary::NormalizedDeltaRotator(AimRotation, DirectRotation);
	return Delta.Yaw;
}

float UPlayerAnimInstance::GetPitchOffset()
{
	if (!IsValid(PlayerRef)) return 0.0f;
	FRotator AimRotation = PlayerRef->GetBaseAimRotation();
	VelocityValue = PlayerRef->GetCharacterMovement()->Velocity;
	FRotator DirectRotation = UKismetMathLibrary::MakeRotFromX(VelocityValue);

	FRotator Delta = UKismetMathLibrary::NormalizedDeltaRotator(AimRotation, DirectRotation);
	return Delta.Pitch;
}