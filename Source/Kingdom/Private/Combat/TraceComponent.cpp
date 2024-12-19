#include "Combat/TraceComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/DamageEvents.h"
#include "Interfaces/HitInterface.h"

UTraceComponent::UTraceComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UTraceComponent::BeginPlay()
{
	Super::BeginPlay();
	SkeletalMesh = GetOwner()->FindComponentByClass<USkeletalMeshComponent>();
}

void UTraceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bIsAttacking) return;

	TArray<FHitResult> OutHitResults;

	if (IsValid(SkeletalMesh))
	{
		FVector StartSocketLocation = SkeletalMesh->GetSocketLocation(Start);
		FVector EndSocketLocation = SkeletalMesh->GetSocketLocation(End);
		FQuat ShapeRotation = SkeletalMesh->GetSocketQuaternion(Rotation);
		double WeaponLength = FVector::Distance(StartSocketLocation, EndSocketLocation);
		FVector BoxHalfExtent = FVector{ BoxCollisionLength, BoxCollisionLength, WeaponLength } * 0.5f ;
		FCollisionShape Box = FCollisionShape::MakeBox(BoxHalfExtent);
		FCollisionQueryParams IgnoreParams = {FName{TEXT("Ignore Params")}, false, Owner};

		bool bHasFoundTarget = GetWorld()->SweepMultiByChannel(
			OutHitResults, StartSocketLocation, EndSocketLocation, ShapeRotation,
			ECollisionChannel::ECC_GameTraceChannel1, Box, IgnoreParams
		);

		if (bDebugMode)
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
	}

	if (OutHitResults.Num() == 0) return;

	float CharacterDamage = 0.0f;

	IHitInterface* HitInterfaceRef = Cast<IHitInterface>(Owner);
	if (HitInterfaceRef)
	{
		CharacterDamage = HitInterfaceRef->GetDamage();
	}

	FDamageEvent TargetAttackedEvent;

	for (const FHitResult& HitResult : OutHitResults)
	{
		AActor* TargetActor = HitResult.GetActor();

		if (TargetToIgnore.Contains(TargetActor)) continue;

		TargetActor->TakeDamage(CharacterDamage, TargetAttackedEvent, Owner->GetInstigatorController(), Owner);

		TargetToIgnore.AddUnique(TargetActor);
	}
}

