#include "Combat/CombatComponent.h"
#include "GameFramework/Character.h"
#include "Character/PlayerClass.h"
#include "Items/ProjectileItems/Projectile.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	ArrowPath = CreateDefaultSubobject<USplineComponent>(TEXT("ArrowPath"));
	ArcEnd = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArcEnd"));
	ArcEnd->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ArcEnd->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	ArcEnd->SetVisibility(false);
	ProjectilePathComponent = CreateDefaultSubobject<USplineMeshComponent>(TEXT("ProjectilePath"));
}

void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();
	CharacterRef = Cast<ACharacter>(GetOwner());
	CreateArrows(InitArrowHold);
}

void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	LineTrace(DeltaTime);
	DrawProjectilePath();
}

void UCombatComponent::DrawProjectilePath()
{
	if (Cast<APlayerClass>(CharacterRef) && Cast<APlayerClass>(CharacterRef)->GetbIsAiming())
	{
		FPredictProjectilePathResult PathResult = ProjectilePath();

		ArcEnd->SetVisibility(true);
		ClearProjectilePath();
		SetSplineMesh(PathResult);
	}
	else if (Cast<APlayerClass>(CharacterRef) && !Cast<APlayerClass>(CharacterRef)->GetbIsAiming())
	{
		ClearProjectilePath();
	}
}

void UCombatComponent::ClearProjectilePath()
{
	ArcEnd->SetVisibility(false);
	for (USplineMeshComponent* Component : SplineMeshes)
	{
		Component->DestroyComponent();
	}
	SplineMeshes.Empty();
	ArrowPath->ClearSplinePoints();
}

FPredictProjectilePathResult UCombatComponent::ProjectilePath()
{
	FPredictProjectilePathParams PredictProjectilePathParams;

	FPredictProjectilePathResult PredictProjectilePathResult;

	PredictProjectilePathParams.StartLocation = ArrowSpawnLocation;
	PredictProjectilePathParams.LaunchVelocity = UKismetMathLibrary::GetForwardVector(ArrowSpawnRotation) * ArrowSpeed;
	PredictProjectilePathParams.bTraceWithCollision = true;
	PredictProjectilePathParams.ProjectileRadius = 5.0f;
	PredictProjectilePathParams.MaxSimTime = 5.0f;
	PredictProjectilePathParams.bTraceWithChannel = true;
	PredictProjectilePathParams.TraceChannel = ECollisionChannel::ECC_Visibility;
	TArray<AActor*> Ignores;
	if (!Ignores.Contains(GetOwner())) Ignores.AddUnique(GetOwner());
	PredictProjectilePathParams.ActorsToIgnore = Ignores;
	PredictProjectilePathParams.SimFrequency = 20.0f;
	EDrawDebugTrace::Type Trace = bIsProjectileDebugLine ? EDrawDebugTrace::ForOneFrame : EDrawDebugTrace::None;
	PredictProjectilePathParams.DrawDebugType = Trace;
	UGameplayStatics::PredictProjectilePath(GetWorld(), PredictProjectilePathParams, PredictProjectilePathResult);

	return PredictProjectilePathResult;
}


void UCombatComponent::SetSplineMesh(FPredictProjectilePathResult& PathResult)
{
	for (const FPredictProjectilePathPointData& Elements : PathResult.PathData)
	{
		ArrowPath->AddSplinePoint(Elements.Location, ESplineCoordinateSpace::Local, true);
	}

	ArrowPath->SetSplinePointType(PathResult.PathData.Num() - 1, ESplinePointType::CurveClamped, true);
	FinalArcLocation = PathResult.PathData.Last().Location;

	for (int i = 0; i <= ArrowPath->GetNumberOfSplinePoints() - 2; i++)
	{
		USplineMeshComponent* SplineMesh = NewObject<USplineMeshComponent>(GetOwner(), FName(("Projectile Path "), i));
		SplineMesh->SetMobility(EComponentMobility::Movable);
		GetOwner()->AddInstanceComponent(SplineMesh);
		SplineMesh->RegisterComponentWithWorld(GetWorld());
		SplineMesh->SetStaticMesh(PathComponent);
		SplineMesh->AttachToComponent(ProjectilePathComponent, FAttachmentTransformRules::KeepRelativeTransform);
		SplineMeshes.Add(SplineMesh);

		FVector StartPos, StartTangent, EndPos, EndTangent;
		ArrowPath->GetLocationAndTangentAtSplinePoint(i, StartPos, StartTangent, ESplineCoordinateSpace::Local);
		ArrowPath->GetLocationAndTangentAtSplinePoint(i + 1, EndPos, EndTangent, ESplineCoordinateSpace::Local);
		SplineMesh->SetStartAndEnd(StartPos, StartTangent, EndPos, EndTangent);

		SplineMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SplineMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		SplineMesh->SetVisibility(true);
	}
	ArcEnd->SetWorldLocation(FinalArcLocation);
}

void UCombatComponent::Player_ComboAttack()
{
	IPlayerInterface* PlayerInterface = nullptr;
	if (CharacterRef->Implements<UPlayerInterface>())
	{
		PlayerInterface = Cast<IPlayerInterface>(CharacterRef);
	}

	if (!PlayerInterface->Execute_HasEnoughStamina(CharacterRef, StaminaToUseForAttacking))
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Not Enought Stamina is remained"));
		return;
	}
	AttackMontage = AttackMontageList[ComboCounter];
	CharacterRef->PlayAnimMontage(AttackMontage);
	ComboCounter++;
	int MaxCombo = AttackMontageList.Num() - 1;
	ComboCounter = UKismetMathLibrary::Wrap(ComboCounter, -1, MaxCombo);

	OnAttackPerformedDelegate.Broadcast(StaminaToUseForAttacking);
}

void UCombatComponent::Player_ProjectileEjection()
{
	APlayerClass* PlayerRef = Cast<APlayerClass>(CharacterRef);

	if (!OutHitResult.bBlockingHit)
	{
		PlayerRef->HandleAttackEnd();
		return;
	}

	PlayArrowFireMontage();
}

void UCombatComponent::PlayArrowFireMontage()
{
	float PlayTime = CharacterRef->PlayAnimMontage(BowFireMontage);
	FTimerHandle TimerHandler;
	GetWorld()->GetTimerManager().SetTimer(TimerHandler, this, &UCombatComponent::SpawnArrow, PlayTime);
}

void UCombatComponent::SpawnArrow()
{
	SpawnTransform = UKismetMathLibrary::MakeTransform(ArrowSpawnLocation, ArrowSpawnRotation);

	AProjectile* SpawnedArrow = GetArrowFromPool();

	if (SpawnedArrow)
	{
		SpawnedArrow->SetProjectileSpeed_Gravity(ArrowSpeed, ArrowSpeed, Gravity);
		SpawnedArrow->SetHidden(false);
		SpawnedArrow->AddDelegate();
		SpawnedArrow->FinishSpawning(SpawnTransform);
	}
}

void UCombatComponent::Enemy_Attack()
{
	if (AttackMontageList.Num() <= 0) return;
	int32 RandomIndex = FMath::RandRange(0, AttackMontageList.Num() - 1);

	AttackMontage = AttackMontageList[RandomIndex];

	Cast<ACharacter>(GetOwner())->PlayAnimMontage(AttackMontage);
}

void UCombatComponent::SetCurrentHoldArrowAmount(int32 Amount)
{
	CreateArrows(Amount);
}

AProjectile* UCombatComponent::GetArrowFromPool()
{
	if (Arrows.Num() > 0)
	{
		AProjectile* Arrow = Arrows.Pop();
		Arrow->SetHidden(false);
		return Arrow;
	}

	return nullptr;
}

void UCombatComponent::LineTrace(float DeltaTime)
{
	FHitResult HitResult;

	APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	FVector CameraLocation = CameraManager->GetCameraLocation();
	FVector CameraForwardVector = (CameraManager->GetActorForwardVector()) * 15000.0f;
	ImpactPoint = CameraLocation + CameraForwardVector;

	EDrawDebugTrace::Type Trace = bIsProjectileDebugLine ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;
	TArray<AActor*> Ignores;
	if (!Ignores.Contains(GetOwner())) Ignores.AddUnique(GetOwner());

	UKismetSystemLibrary::LineTraceSingle(GetWorld(), CameraLocation, ImpactPoint, ETraceTypeQuery::TraceTypeQuery1, true, Ignores, Trace, HitResult, true);

	if (HitResult.bBlockingHit)
	{
		OutHitResult = HitResult;

		ImpactPoint = OutHitResult.ImpactPoint;
	}

	if (IsValid(CharacterRef))
	{
		APlayerClass* PlayerRef = nullptr;
		if (Cast<APlayerClass>(CharacterRef))
		{
			PlayerRef = Cast<APlayerClass>(CharacterRef);
			if (IsValid(PlayerRef->GetCurrentEquippedWeapon()))
			{
				USkeletalMeshComponent* Mesh = PlayerRef->GetCurrentEquippedWeapon()->FindComponentByClass<USkeletalMeshComponent>();
				if (Mesh)
				{
					SpawnTransform = Mesh->GetSocketTransform(FName("Projectile"));
				}
			}
		}
	}

	TargetArrowSpawnLocation = SpawnTransform.GetLocation();
	FVector Direction = ImpactPoint - TargetArrowSpawnLocation;
	TargetArrowSpawnRotation = UKismetMathLibrary::MakeRotFromX(Direction);

	ArrowSpawnLocation = FMath::VInterpTo(ArrowSpawnLocation, TargetArrowSpawnLocation, DeltaTime, 30.0f);
	ArrowSpawnRotation = FMath::RInterpTo(ArrowSpawnRotation, TargetArrowSpawnRotation, DeltaTime, 30.0f);
}

void UCombatComponent::CreateArrows(int32 Amount)
{
	for (int32 i = 0; i < Amount; i++)
	{
		AProjectile* SpawnedArrow = GetWorld()->SpawnActorDeferred<AProjectile>(
			ArrowBlueprint.Get(), FTransform::Identity, GetOwner(), GetOwner<APawn>(),
			ESpawnActorCollisionHandlingMethod::Undefined);
		if (SpawnedArrow)
		{
			Arrows.Add(SpawnedArrow);
			SpawnedArrow->SetHidden(true);
		}		
	}
}
