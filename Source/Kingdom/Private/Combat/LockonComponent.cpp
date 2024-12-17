#include "Combat/LockonComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "Interfaces/EnemyInterface.h"

ULockonComponent::ULockonComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void ULockonComponent::BeginPlay()
{
	Super::BeginPlay();
	Player = Cast<ACharacter>(GetOwner());
	PlayerController = GetWorld()->GetFirstPlayerController();
	MoveComponent = Player->GetCharacterMovement();
	SpringArmComponent = Player->FindComponentByClass<USpringArmComponent>();
}

void ULockonComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (!IsValid(CurrentTargetActor)) return;

	FVector CurrentLocation = Player->GetActorLocation();
	FVector CurrentTargetLocation = CurrentTargetActor->GetActorLocation();

	double TargetDistance = FVector::Distance(CurrentLocation, CurrentTargetLocation);

	if (TargetDistance >= BreakDistance)
	{
		EndLockon();
		return;
	}

	CurrentTargetLocation.Z -= 125.0f;

	FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(CurrentLocation, CurrentTargetLocation);
	PlayerController->SetControlRotation(NewRotation);
}

void ULockonComponent::StartLockon()
{
	FHitResult OutHitResult;

	bool bHasFoundTarget = TraceProcess_FindTargetActor(OutHitResult);

	if (!bHasFoundTarget) return;
	if (!OutHitResult.GetActor()->Implements<UEnemyInterface>()) return;

	CurrentTargetActor = OutHitResult.GetActor();
	PlayerController->SetIgnoreLookInput(true);
	MoveComponent->bOrientRotationToMovement = false;
	MoveComponent->bUseControllerDesiredRotation = true;
	SpringArmComponent->TargetOffset = FVector{ 0.0f, 0.0f, 100.0f };

	IEnemyInterface::Execute_OnTargeted(CurrentTargetActor);

	if (CurrentTargetActor)
	{
		OnUpdateTargetDelegate.Broadcast(CurrentTargetActor);
		UE_LOG(LogTemp, Warning, TEXT("Broacast::Target : %s"), *CurrentTargetActor->GetName());
		return;
	}
}

void ULockonComponent::EndLockon()
{
	if (!IsValid(CurrentTargetActor)) return;
	if (!CurrentTargetActor->Implements<UEnemyInterface>()) { return; }
	IEnemyInterface::Execute_UnTargeted(CurrentTargetActor);

	CurrentTargetActor = nullptr;
	MoveComponent->bOrientRotationToMovement = true;
	MoveComponent->bUseControllerDesiredRotation = false;
	SpringArmComponent->TargetOffset = FVector::ZeroVector;
	PlayerController->ResetIgnoreLookInput();

	if (CurrentTargetActor == nullptr)
	{
		OnUpdateTargetDelegate.Broadcast(CurrentTargetActor);
		return;
	}
}

bool ULockonComponent::TraceProcess_FindTargetActor(FHitResult OutResult)
{
	FVector CurrentLocation = GetOwner()->GetActorLocation();
	FCollisionShape Sphere = FCollisionShape::MakeSphere(TraceSphereRadius);
	FCollisionQueryParams IgnoreActorParams{ FName(TEXT("IgnoreCollisionParams")), false, GetOwner() };

	return GetWorld()->SweepSingleByChannel(
		OutResult, CurrentLocation, CurrentLocation,
		FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel1,
		Sphere, IgnoreActorParams);
}



