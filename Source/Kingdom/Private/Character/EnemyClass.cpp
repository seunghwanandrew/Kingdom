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
#include "Items/EquipableItems/EquipableItemClass.h"
#include "Items/PickableItems/PickableItemClass.h"

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
	if (IsValid(EnemyController))
	{
		EnemyController->GetBlackboardComponent()->SetValueAsObject(FName("Target"), DetectedTarget);
	}
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

void AEnemyClass::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnemyClass::Attack()
{
	EPlayerActionState State;
	if (DetectedTarget->Implements<UPlayerInterface>())
	{
		IPlayerInterface* PlayerRef = Cast<IPlayerInterface>(DetectedTarget);
		State = PlayerRef->GetPlayerActionState_Implementation();
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

bool AEnemyClass::IsBoss_Implementation()
{
	return this->ActorHasTag(FName("Boss")) ? true : false;
}

void AEnemyClass::Initialize()
{
	Super::Initialize();

	GetCapsuleComponent()->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel2);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Block);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3, ECollisionResponse::ECR_Ignore);

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	MoveComponent->MaxWalkSpeed = 55.0f;
	MoveComponent->MaxAcceleration = 500.0f;
	Tags.Add(FName("Enemy"));
}

void AEnemyClass::CreateInitComponent()
{
	Super::CreateInitComponent();

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
	PawnSensing->SightRadius = 800.0f;
	PawnSensing->HearingThreshold = 2800.0f;
	PawnSensing->SetPeripheralVisionAngle(40.0f);
}

void AEnemyClass::OnSeePawn(APawn* Pawn)
{
	if (!IsValid(Pawn)) return;

	if (Pawn->Implements<UPlayerInterface>())
	{
		IPlayerInterface* PlayerRef = Cast<IPlayerInterface>(Pawn);
		if (PlayerRef->GetPlayerActionState_Implementation() == EPlayerActionState::EPAS_Death) return;
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

	float RandFloat = FMath::FRand();
	if (RandFloat <= RandomSpawnRate)
	{
		int32 RandomIndex = FMath::RandRange(0, PossibleDropItem.Num() - 1);
		FVector AdjustSpawnPos = FVector{ 0.0f, 0.0f, 150.0f };
		FTransform SpawnTransform = UKismetMathLibrary::MakeTransform(GetActorLocation() + AdjustSpawnPos, GetActorRotation());

		APickableItemClass* DropItem = GetWorld()->SpawnActorDeferred<APickableItemClass>(PossibleDropItem[RandomIndex], SpawnTransform);

		if (DropItem)
		{
			DropItem->SetSoulValue(Souls);			
			DropItem->SetItemState(EItemState::EIS_Rotating);
			DropItem->FinishSpawning(SpawnTransform);
			DropItem->AbleNiagraComponent();
		}
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