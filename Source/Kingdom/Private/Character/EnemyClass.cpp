#include "Character/EnemyClass.h"
#include "AI/Controller/EnemyController.h"
#include "AnimInstances/EnemyAnimInstance.h"
#include "BrainComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/PlayerClass.h"
#include "Character/StatsComponent.h"
#include "Combat/CombatComponent.h"
#include "Combat/TraceComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interfaces/PlayerInterface.h"
#include "Kismet/KismetMathLibrary.h"
#include "MotionWarpingComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "UserWidget/TargetDisplayerWidgetComponent.h"
#include "Items/EquipableItems/EquipableItemClass.h"

UTraceComponent* AEnemyClass::GetCurrentEquipWeaponTraceComponent()
{
	return DefaultWeapon->GetTraceComponent();
}

AEnemyClass::AEnemyClass()
{
	PrimaryActorTick.bCanEverTick = true;
	Initialize();
	CreateInitComponent();
	CreateCombatComponent();
}

void AEnemyClass::BeginPlay()
{
	Super::BeginPlay();
	EnemyController = Cast<AEnemyController>(GetController());
	EnemyController->GetBlackboardComponent()->SetValueAsObject(FName("Target"), DetectedTarget);
	AnimInstance = Cast<UEnemyAnimInstance>(GetMesh()->GetAnimInstance());
	PawnSensing->OnSeePawn.AddDynamic(this, &AEnemyClass::OnSeePawn);
	StatsComponent->OnDeathDelegate.AddDynamic(this, &AEnemyClass::OnDeath);
	HealthBarComponent->SetHealthWidget(StatsComponent->GetCurrentHealthPercentage());
	if (IsValid(Weapon))
	{
		DefaultWeapon = GetWorld()->SpawnActor<AEquipableItemClass>(Weapon);
		DefaultWeapon->SetActorLocation(FVector::ZeroVector);
		FAttachmentTransformRules TransformRules(EAttachmentRule::KeepRelative, true);
		DefaultWeapon->AttachToComponent(GetMesh(), TransformRules, FName("Weapon"));
		DefaultWeapon->GetTraceComponent()->SetOwner(this);
	}
}

void AEnemyClass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!IsValid(DetectedTarget)) return;

	if (!InRange(DetectedTarget, PawnSensing->SightRadius))
	{
		DetectedTarget = nullptr;
		OnTargetDectectedDelegate.Broadcast(DetectedTarget, false);
		if (HealthBarComponent->IsWidgetVisible())
		{
			SetHealthBarVisibility(false);
		}
	}
	SetWarpTargetTranslation();
}

void AEnemyClass::OnTargeted_Implementation()
{
	TargetDisplayer->SetVisibility(true);
	HealthBarComponent->SetVisibility(true);
}

void AEnemyClass::UnTargeted_Implementation()
{
	TargetDisplayer->SetVisibility(false);
	HealthBarComponent->SetVisibility(false);
}

void AEnemyClass::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnemyClass::Attack()
{
	EPlayerActionState State;
	if (DetectedTarget->Implements<UPlayerInterface>())
	{
		APlayerClass* PlayerRef = Cast<APlayerClass>(DetectedTarget);
		State = PlayerRef->GetActionState();
	}
	if (State == EPlayerActionState::EPAS_Death)
	{
		DetectedTarget = nullptr;
		OnTargetDectectedDelegate.Broadcast(DetectedTarget, false);
		return;
	}
	
	CombatComponent->Enemy_Attack();
}

void AEnemyClass::HandleAttackEnd()
{
	if (IsValid(Weapon))
	{
		DefaultWeapon->Set_EmptyTraceTargetToIgnoreVariable();
	}
	else
	{
		TraceComponent->SetTargetToIgnoreArrayEmpty();
	}
}

FVector AEnemyClass::RandomDestinationLocation()
{
	if (RoamingTargetDestination.Num() <= 0) return FVector::ZeroVector;

	int32 RandomIndex = FMath::RandRange(0, RoamingTargetDestination.Num() - 1);

	AActor* TargetDestination = RoamingTargetDestination[RandomIndex];

	return TargetDestination->GetActorLocation();
}

UAnimMontage* AEnemyClass::GetTransformPlayMontage()
{
	SelectRandomTransformPlayMontage();
	return MontageToTransformPlay;
}

void AEnemyClass::SetTransfromPlayMontage(UAnimMontage* MontageToPlay)
{
	MontageToTransformPlay = MontageToPlay;
}

void AEnemyClass::SelectRandomTransformPlayMontage()
{
	int32 RandomIndex = FMath::RandRange(0, TransformMotionList.Num() - 1);
	MontageToTransformPlay = TransformMotionList[RandomIndex];
}

void AEnemyClass::GetHit_Implementation(float DamageAmount, AController* EventInstigator, AActor* DamageCauser, FVector ImpactPoint)
{
	Super::GetHit_Implementation(DamageAmount, EventInstigator, DamageCauser, ImpactPoint);
	// To Do Hit Reaction;
	SetHealthBarVisibility(true);

	DetectedTarget = DamageCauser;
	OnTargetDectectedDelegate.Broadcast(DetectedTarget, true);

	HealthBarComponent->SetHealthWidget(StatsComponent->GetCurrentHealthPercentage());
}

void AEnemyClass::Initialize()
{
	Super::Initialize();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	MoveComponent->MaxWalkSpeed = 55.0f;
	MoveComponent->MaxAcceleration = 500.0f;
	Tags.Add(FName("Enemy"));
}

void AEnemyClass::CreateInitComponent()
{
	Super::CreateInitComponent();
	TargetDisplayer = CreateDefaultSubobject<UTargetDisplayerWidgetComponent>(TEXT("TargetDisplayer"));
	TargetDisplayer->SetupAttachment(GetRootComponent());
	TargetDisplayer->SetWidgetSpace(EWidgetSpace::Screen);
	TargetDisplayer->SetVisibility(false);

	HealthBarComponent = CreateDefaultSubobject<UEnemyHealthBarComponent>(TEXT("HealthBarDisplayer"));
	HealthBarComponent->SetupAttachment(GetRootComponent());
	HealthBarComponent->SetWidgetSpace(EWidgetSpace::Screen);
	HealthBarComponent->SetVisibility(false);
}

void AEnemyClass::CreateCombatComponent()
{
	Super::CreateCombatComponent();
	TraceComponent = CreateDefaultSubobject<UTraceComponent>(TEXT("TraceComponent"));
	MotionWarping = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarping"));

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	PawnSensing->SightRadius = 1200.0f;
	PawnSensing->HearingThreshold = 600.0f;
	PawnSensing->SetPeripheralVisionAngle(60.0f);
}

void AEnemyClass::OnSeePawn(APawn* Pawn)
{
	if (!IsValid(Pawn)) return;

	if (Pawn->Implements<UPlayerInterface>())
	{
		DetectedTarget = Cast<AActor>(Pawn);
		OnTargetDectectedDelegate.Broadcast(DetectedTarget, true);
	}
}

void AEnemyClass::OnDeath(int32 Index)
{
	bIsDead = true;
	AnimInstance->bIsDead = true;
	switch (Index)
	{
	case 0:
		AnimInstance->SetDeathPose(EDeathPose::EDP_Death_01);
		break;
	case 1:
		AnimInstance->SetDeathPose(EDeathPose::EDP_Death_02);
		break;
	}
	OnDeathNotifyDelegate.Broadcast();
	if (IsValid(DefaultWeapon))
	{
		DefaultWeapon->Destroy();
	}
	SetLifeSpan(3.0f);
	Cast<AEnemyController>(GetController())->GetBrainComponent()->StopLogic(FString(TEXT("Defeated")));
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AEnemyClass::SetWarpTargetTranslation()
{
	if (!IsValid(DetectedTarget)) return;
	FTransform TargetTransform = DetectedTarget->GetActorTransform();

	MotionWarping->AddOrUpdateWarpTargetFromTransform(FName("WarpTargetTranslation"), TargetTransform);
}

bool AEnemyClass::InRange(AActor* Target, float AcceptableRadius)
{
	FVector TargetLocation = Target->GetActorLocation();
	FVector CurrentLocation = GetActorLocation();

	float Distance = (CurrentLocation - TargetLocation).Length();

	return Distance <= AcceptableRadius;
}