#include "AnimInstances/PlayerAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/PlayerClass.h"

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	bShouldMove = IsShouldMove();
	bIsFalling = IsFalling();
	GroundSpeed = SetGroundSpeed();
	GroundDirection = SetGroundDirection();

	UE_LOG(LogTemp, Warning, TEXT("Combat state : %s"), CombatState == EPlayerActionState::EPAS_Combat ? TEXT("Combat") : TEXT("None"));
}

void UPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	PlayerRef = Cast<APlayerClass>(TryGetPawnOwner());
}

void UPlayerAnimInstance::OnUpdateTargetHandle()
{
	CombatState = PlayerRef->GetCombatState();
	UE_LOG(LogTemp, Warning, TEXT("Combat state : %s"), CombatState == EPlayerActionState::EPAS_Combat ? TEXT("Combat") : TEXT("None"));
}

bool UPlayerAnimInstance::IsShouldMove()
{
	if (!PlayerRef) return false;
	UCharacterMovementComponent* MoveComponent = PlayerRef->GetMoveComponent();
	FVector CurrentAcceleration = MoveComponent->GetCurrentAcceleration();
	return (CurrentAcceleration != FVector::ZeroVector && GroundSpeed > 3.0f);
}

bool UPlayerAnimInstance::IsFalling()
{
	if (!PlayerRef) return false;
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
