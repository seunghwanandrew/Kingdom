#include "AnimInstances/EnemyAnimInstance.h"
#include "Character/EnemyClass.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AI/Controller/EnemyController.h"

void UEnemyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	EnemyRef = Cast<ACharacter>(TryGetPawnOwner());
}

void UEnemyAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);
	SetGroundSpeed();
	SetLookRotation();
}

void UEnemyAnimInstance::SetGroundSpeed()
{
	if (!IsValid(EnemyRef)) return;
	Velocity = EnemyRef->GetVelocity();
	GroundSpeed = static_cast<float>(Velocity.Length());
}

void UEnemyAnimInstance::SetLookRotation()
{
	if (!IsValid(EnemyRef)) return;
	AController* Controller = EnemyRef->GetController();
	if (!IsValid(Controller)) return;
	AActor* Target = Cast<AEnemyController>(Controller)->GetTargetActor();
	if (!IsValid(Target)) return;
	FRotator Rotation = Controller->GetControlRotation();
	FRotator YawRotation = { 0.0f, Rotation.Yaw, 0.0f };
	FVector Forward = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	FVector Distance = Target->GetActorLocation() - EnemyRef->GetActorLocation();

	FVector NormalizedForward = Forward.GetSafeNormal();
	FVector NormalizedDistance = Distance.GetSafeNormal();

	float DotProduct = FVector::DotProduct(NormalizedForward, NormalizedDistance);
	
	float Angle = FMath::Acos(DotProduct);
	float AngleDegree = FMath::RadiansToDegrees(Angle);

	LookRotation = AngleDegree;
}
