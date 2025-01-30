#include "Combat/LockonComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "Character/PlayerClass.h"
#include "Character/EnemyClass.h"
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
	if (IsValid(CurrentTargetActor))
	{
		FVector CurrentLocation = Player->GetActorLocation();
		FVector CurrentTargetLocation = CurrentTargetActor->GetActorLocation();
		AutomaticEndLockonProcess(CurrentLocation, CurrentTargetLocation);

		FRotator NewRotation = CalculateCombatTargetRotation(CurrentLocation, CurrentTargetLocation);		
		PlayerController->SetControlRotation(NewRotation);
	}	
}

void ULockonComponent::StartLockon()
{
	bool bHasFoundTarget = TraceProcess_FindTargetActor(OutHitResult);

	if (!bHasFoundTarget) return;
	if (!OutHitResult.GetActor()->Implements<UEnemyInterface>()) return;

	if (OutHitResult.GetActor()->ActorHasTag("Boss"))
	{
		CurrentTargetActor = OutHitResult.GetActor();
		APlayerClass* PlayerRef = Cast<APlayerClass>(Player);
		PlayerRef->SetIsEngaging(true);
		Cast<AEnemyClass>(CurrentTargetActor)->OnDeathNotifyDelegate.AddDynamic(this, &ULockonComponent::OnTargetDead);
		Cast<AEnemyClass>(CurrentTargetActor)->SetHealthBarVisibility(true);
		SceneUpdate(FVector{ 0.0f, 0.0f, 100.0f }, false, true, true);

		IEnemyInterface::Execute_OnTargeted(CurrentTargetActor);

		BroadCastCurrentTarget(CurrentTargetActor, DebugMessageOn);
	}
}

void ULockonComponent::StartLockon(AActor* Target)
{
	APlayerClass* PlayerRef = Cast<APlayerClass>(Player);
	PlayerRef->SetIsEngaging(true);
	CurrentTargetActor = Target;
	Cast<AEnemyClass>(CurrentTargetActor)->OnDeathNotifyDelegate.AddDynamic(this, &ULockonComponent::OnTargetDead);
	Cast<AEnemyClass>(CurrentTargetActor)->SetHealthBarVisibility(true);
	SceneUpdate(FVector{ 0.0f, 0.0f, 100.0f }, false, true, true);

	IEnemyInterface::Execute_OnTargeted(CurrentTargetActor);

	BroadCastCurrentTarget(CurrentTargetActor, DebugMessageOn);
}

void ULockonComponent::EndLockon()
{
	if (!IsValid(CurrentTargetActor)) return;
	if (!CurrentTargetActor->Implements<UEnemyInterface>()) { return; }

	Cast<AEnemyClass>(CurrentTargetActor)->OnDeathNotifyDelegate.RemoveDynamic(this, &ULockonComponent::OnTargetDead);
	Cast<AEnemyClass>(CurrentTargetActor)->SetHealthBarVisibility(false);
	IEnemyInterface::Execute_UnTargeted(CurrentTargetActor);
	CurrentTargetActor = nullptr;
	SceneUpdate(FVector::ZeroVector, true, false, false);
	BroadCastCurrentTarget(CurrentTargetActor, DebugMessageOn);
}

void ULockonComponent::OnTargetDead()
{
	IEnemyInterface::Execute_UnTargeted(CurrentTargetActor);
	CurrentTargetActor = nullptr;
	SceneUpdate(FVector::ZeroVector, true, false, false);
	OnUpdateTargetDelegate.Broadcast(CurrentTargetActor);
}

bool ULockonComponent::TraceProcess_FindTargetActor(FHitResult OutResult)
{
	FVector CurrentLocation = GetOwner()->GetActorLocation();
	FCollisionShape Sphere = FCollisionShape::MakeSphere(TraceSphereRadius);
	FCollisionQueryParams IgnoreActorParams{ FName(TEXT("IgnoreCollisionParams")), false, GetOwner() };

	bool bFoundTarget = GetWorld()->SweepSingleByChannel(
		OutResult, CurrentLocation, CurrentLocation,
		FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel1,
		Sphere, IgnoreActorParams);

	if (bFoundTarget)
	{
		OutHitResult = OutResult;
	}

	return bFoundTarget;
}

void ULockonComponent::SceneUpdate(FVector TargetOffset, bool OrientRotationToMovement, bool UseControllerDesiredRotation, bool IgnoreLookInput)
{
	if (IgnoreLookInput == true)
	{
		PlayerController->SetIgnoreLookInput(IgnoreLookInput);
	}
	else
	{
		PlayerController->ResetIgnoreLookInput();
	}
	MoveComponent->bOrientRotationToMovement = OrientRotationToMovement;
	MoveComponent->bUseControllerDesiredRotation = UseControllerDesiredRotation;
	SpringArmComponent->TargetOffset = TargetOffset;
}

void ULockonComponent::BroadCastCurrentTarget(AActor* Target, bool bDebugMessage)
{
	OnUpdateTargetDelegate.Broadcast(Target);
	if (bDebugMessage)
	{
		if (!GEngine) return;
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Orange, (TEXT("Broacast::Target : %s"), *Target->GetName()));
	}
}

void ULockonComponent::AutomaticEndLockonProcess(FVector Location1, FVector Location2)
{
	double TargetDistance = FVector::Distance(Location1, Location2);

	if (TargetDistance >= BreakDistance)
	{
		EndLockon();
	}
}

FRotator ULockonComponent::CalculateCombatTargetRotation(FVector Location1, FVector Location2)
{
	Location2.Z -= 125.0f;
	UKismetMathLibrary::FindLookAtRotation(Location1, Location2);
	return UKismetMathLibrary::FindLookAtRotation(Location1, Location2);
}


