#include "Combat/TraceComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Interfaces/HitInterface.h"
#include "Combat/TraceSockets.h"
#include "GameFramework/Actor.h"
#include "GeometryCollection/GeometryCollectionComponent.h"

UTraceComponent::UTraceComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UTraceComponent::BeginPlay()
{
	Super::BeginPlay();
	if (!IsValid(Owner)) Owner = GetOwner();
	SkeletalMesh = GetOwner()->FindComponentByClass<USkeletalMeshComponent>();
}

void UTraceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bIsAttacking) return;

	TArray<FHitResult> OutHitResults;
	BoxTraceProcess_FindTargetActor();
	float CharacterDamage = GetAttackPowerFromOwner(Owner);
	ApplyDamageToFoundActor(CharacterDamage);
}

void UTraceComponent::BoxTraceProcess_FindTargetActor()
{
	if (!IsValid(SkeletalMesh)) return;

	for (const FTraceSockets& Socket : SocketList)
	{
		FVector StartSocketLocation = SkeletalMesh->GetSocketLocation(Socket.Start);
		FVector EndSocketLocation = SkeletalMesh->GetSocketLocation(Socket.End);
		FQuat ShapeRotation = SkeletalMesh->GetSocketQuaternion(Socket.Rotation);

		double WeaponLength = FVector::Distance(StartSocketLocation, EndSocketLocation);
		FVector BoxHalfExtent = FVector{ BoxCollisionLength, BoxCollisionLength, WeaponLength } * 0.5f;
		FCollisionShape Box = FCollisionShape::MakeBox(BoxHalfExtent);

		FCollisionQueryParams IgnoreParams = { FName{ TEXT("Ignore Params") }, false, Owner };

		bool bHasFoundTarget = GetWorld()->SweepMultiByChannel(
			OutResults, StartSocketLocation, EndSocketLocation, ShapeRotation,
			ECollisionChannel::ECC_GameTraceChannel1, Box, IgnoreParams
		);

		if (bIsDebugMode)
		{
			DrawDebugBoxTrace(StartSocketLocation, EndSocketLocation, Box, bHasFoundTarget, ShapeRotation);
		}
	}
}

void UTraceComponent::DrawDebugBoxTrace(const FVector& StartSocketLocation, const FVector& EndSocketLocation, FCollisionShape& Box, bool bHasFoundTarget, FQuat& ShapeRotation)
{
	if (IsValid(GetWorld()))
	{
		FVector CenterPoint = UKismetMathLibrary::VLerp(StartSocketLocation, EndSocketLocation, 0.5f);
		UKismetSystemLibrary::DrawDebugBox(
			GetWorld(), CenterPoint, Box.GetExtent(),
			bHasFoundTarget ? FLinearColor::Green : FLinearColor::Red,
			ShapeRotation.Rotator(), 1.0f, 2.0f
		);
	}
}

float UTraceComponent::GetAttackPowerFromOwner(const AActor* OwnerRef)
{
	float CharacterDamage = 0.0f;

	if (Owner->Implements<UHitInterface>())
	{
		IHitInterface* HitInterface = Cast<IHitInterface>(Owner);
		if (HitInterface)
		{
			CharacterDamage = HitInterface->Execute_GetCharacterStrength(Owner);
		}
	}

	return CharacterDamage;
}

void UTraceComponent::ApplyDamageToFoundActor(float CharacterDamage)
{
	for (const FHitResult& HitResult : OutResults)
	{
		AActor* TargetActor = HitResult.GetActor();
		FVector ImpactPoint = HitResult.ImpactPoint;
		if (TargetToIgnore.Contains(TargetActor)) continue;

		if (Owner->ActorHasTag("Enemy") && TargetActor->ActorHasTag("Enemy")) continue;
		bool IsTrue = TargetActor->Implements<UHitInterface>();

		if (IsTrue)
		{
			IHitInterface* HitInterface = Cast<IHitInterface>(TargetActor);
			HitInterface->Execute_GetHit(TargetActor, CharacterDamage, Owner->GetInstigatorController(), Owner, ImpactPoint);
		}
		TargetToIgnore.AddUnique(TargetActor);
	}
}