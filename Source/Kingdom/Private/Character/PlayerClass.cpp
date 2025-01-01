#include "Character/PlayerClass.h"
#include "AnimInstances/PlayerAnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Character/ActionComponent.h"
#include "Character/StatsComponent.h"
#include "Combat/LockonComponent.h"
#include "Combat/CombatComponent.h"
#include "Combat/TraceComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Items/EquipableItems/CloseRangedWeaponClass.h"

APlayerClass::APlayerClass()
{
	PrimaryActorTick.bCanEverTick = true;
	Initialize();	
	CreateInitComponent();
	CreateAppearanceComponent();
	CreateCombatComponent();
}

void APlayerClass::BeginPlay()
{
	Super::BeginPlay();
	
	AnimInstance = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());

	LockonComponent->OnUpdateTargetDelegate.AddDynamic(this, &APlayerClass::OnUpdateTargetHandler);
	CombatComponent->OnAttackPerformedDelegate.AddDynamic(this, &APlayerClass::OnAttackPerformedHandler);
	ActionComponent->OnMovementStaminaUsageDelegate.AddDynamic(this, &APlayerClass::OnMovementStaminaHandler);
	StatsComponent->OnDeathDelegate.AddDynamic(this, &APlayerClass::OnDeath);
}

void APlayerClass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	StatsComponent->StaminaRegenerating(DeltaTime);
}

void APlayerClass::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (!IsValid(PlayerController)) return;
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	if (Subsystem)
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}	
}

void APlayerClass::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (!EnhancedInput) return;

	EnhancedInput->BindAction(JumpAction, ETriggerEvent::Started, this, &APlayerClass::Jump);
	EnhancedInput->BindAction(JumpAction, ETriggerEvent::Completed, this, &APlayerClass::StopJumping);
	EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerClass::Move);
	EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerClass::Look);
	EnhancedInput->BindAction(LockonAction, ETriggerEvent::Started, this, &APlayerClass::LockonProcess);
	EnhancedInput->BindAction(AttackAction, ETriggerEvent::Started, this, &APlayerClass::ComboAttackProcess);
	EnhancedInput->BindAction(EKeyPressedAction, ETriggerEvent::Started, this, &APlayerClass::EKeyPressed);
	EnhancedInput->BindAction(CrouchAction, ETriggerEvent::Triggered, this, &APlayerClass::StealthMovement);
	EnhancedInput->BindAction(CrouchAction, ETriggerEvent::Completed, this, &APlayerClass::StealthToWalkMovement);
	EnhancedInput->BindAction(SprintModeControlKey, ETriggerEvent::Triggered, this, &APlayerClass::SprintModeControl);
	EnhancedInput->BindAction(RollAction, ETriggerEvent::Triggered, this, &APlayerClass::Rolling);
}

void APlayerClass::OnUpdateTargetHandler(AActor* TargetActor)
{
	if (TargetActor)
	{
		if (CurrentEquippedWeapon == CloseRangedWeaponItem && ActionState != EPlayerActionState::EPAS_Combat)
		{
			EquipCloseRangeWeaponProcess();
		}
		AnimInstance->SetActionState(ActionState);
		SetWeaponToEquip_UnEquipSocketPoint(CurrentEquippedWeapon, CloseRangedWeaponEquipSocketName);
	}
	else
	{
		UnEquipCloseRangeWeaponProcess();
	}
	AnimInstance->OnUpdateTargetHandle();
}

void APlayerClass::OnAttackPerformedHandler(float AttackStamina)
{
	StatsComponent->ApplyStaminaUsage(AttackStamina);
}

void APlayerClass::OnMovementStaminaHandler(float StaminaUsage)
{
	StatsComponent->ApplyStaminaUsage(StaminaUsage);
}

void APlayerClass::OnDeath(int32 Index)
{
	DisableInput(GetController<APlayerController>());
	ActionState = EPlayerActionState::EPAS_Death;
	AnimInstance->SetActionState(ActionState);
	switch (Index)
	{
	case 0:
		AnimInstance->SetDeathPose(EDeathPose::EDP_Death_01);
		break;
	case 1:
		AnimInstance->SetDeathPose(EDeathPose::EDP_Death_02);
		break;
	case 2:
		AnimInstance->SetDeathPose(EDeathPose::EDP_Death_03);
		break;
	case 3:
		AnimInstance->SetDeathPose(EDeathPose::EDP_Death_04);
		break;
	}	
	LockonComponent->EndLockon();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// TODO : Play Death Widget
}

void APlayerClass::SetOverlappedItem_Implementation(AActor* OverlappedItemRef)
{
	OverlappedItem = OverlappedItemRef;
}

void APlayerClass::HandleAttackEnd()
{
	SetActionState(EPlayerActionState::EPAS_Combat);
	AnimInstance->SetActionState(EPlayerActionState::EPAS_Combat);
	CurrentEquippedWeapon->Set_EmptyTraceTargetToIgnoreVariable();
}

UTraceComponent* APlayerClass::GetCurrentEquipWeaponTraceComponent()
{
	return CurrentEquippedWeapon->GetTraceComponent();
}

void APlayerClass::UnEquipPoint()
{
	FName CurrentWeaponEquipPointSocketName = GetCurrentEquippedSocketName(CurrentEquippedWeapon);
	SwitchingWeaponEquipSocket(CurrentEquippedWeapon, CurrentWeaponEquipPointSocketName);
}


void APlayerClass::GetHit_Implementation(float DamageAmount, AController* EventInstigator, AActor* DamageCauser, FVector ImpactPoint)
{
	if (ActionComponent->IsRollingMode()) return;

	Super::GetHit_Implementation(DamageAmount, EventInstigator, DamageCauser, ImpactPoint);
	if (!bIsEngaging && DamageCauser->ActorHasTag("Boss"))
	{
		LockonComponent->StartLockon(DamageCauser);
	}
	if (ActionState == EPlayerActionState::EPAS_Attacking)
	{
		ActionState = EPlayerActionState::EPAS_Combat;
	}
}

bool APlayerClass::HasEnoughStamina_Implementation(float MinValue)
{
	return StatsComponent->Stats[EStats::ES_Stamina] >= MinValue;
}

void APlayerClass::TargetDeathProcess_Implementation()
{
	LockonComponent->EndLockon();
}

void APlayerClass::Move(const FInputActionValue& Value)
{
	if (ActionState <= EPlayerActionState::EPAS_Attacking) return;
	FVector2D MoveVector = Value.Get<FVector2D>();

	if (!Controller) return;
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation{ 0.0f, Rotation.Yaw, 0.0f };

	const FVector FowardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(FowardDirection, MoveVector.Y);
	AddMovementInput(RightDirection, MoveVector.X);
}

void APlayerClass::Look(const FInputActionValue& Value)
{
	FVector2D LookVector = Value.Get<FVector2D>();
	if (!Controller) return;
	AddControllerYawInput(LookVector.X);
	AddControllerPitchInput(LookVector.Y);
}

void APlayerClass::Jump()
{
	if (ActionComponent->IsRollingMode()) return;
	Super::Jump();
}

void APlayerClass::StopJumping()
{
	Super::StopJumping();
}

void APlayerClass::EKeyPressed()
{
	if (IsValid(OverlappedItem))
	{
		if (OverlappedItem->ActorHasTag("CloseRangedWeaponClass"))
		{
			StoreWeaponItem(OverlappedItem);	
		}
		else return;
	}
	else if (IsValid(CurrentEquippedWeapon))
	{
		if (ActionState <= EPlayerActionState::EPAS_Attacking) return;
		if (bIsEngaging) return;

		if (CurrentEquippedWeapon == CloseRangedWeaponItem)
		{
			FName CurrentSocketName = GetCurrentEquippedSocketName(CurrentEquippedWeapon);
			if (AnimInstance)
			{
				if (ActionState == EPlayerActionState::EPAS_None)
				{
					EquipCloseRangeWeaponProcess();
					SwitchingWeaponEquipSocket(CurrentEquippedWeapon, CurrentSocketName);
				}
				else if (ActionState == EPlayerActionState::EPAS_Combat)
				{
					UnEquipCloseRangeWeaponProcess();
				}
			}
		}
	}
}

void APlayerClass::LockonProcess()
{
	if (!IsValid(CurrentEquippedWeapon)) return;
	if (!bIsEngaging)
	{
		LockonComponent->StartLockon();
	}
	else
	{
		LockonComponent->EndLockon();
	}
}

void APlayerClass::ComboAttackProcess()
{
	if (!IsValid(CombatComponent) || ActionState != EPlayerActionState::EPAS_Combat) return;
	ActionState = EPlayerActionState::EPAS_Attacking;
	CombatComponent->Player_ComboAttack();
}

void APlayerClass::StealthMovement()
{
	ActionComponent->StealthMovement();
}

void APlayerClass::StealthToWalkMovement()
{
	ActionComponent->BasicWalkMovement();
}

void APlayerClass::SprintModeControl()
{
	bool Sprinting = ActionComponent->IsSprintMode();

	if (!Sprinting)
	{
		ActionComponent->SprintMovement();
	}
	else
	{
		ActionComponent->SetSprintMode(false);
		ActionComponent->BasicWalkMovement();
	}
}

void APlayerClass::Rolling()
{
	if (MoveComponent->IsFalling()) return;
	ActionComponent->Roll();
}

void APlayerClass::SwitchingWeaponEquipSocket(AEquipableItemClass* EquipmentItem, FName CurrentSocketName)
{
	if (!IsValid(EquipmentItem) || CurrentSocketName == FName("")) return;
	if (ActionState == EPlayerActionState::EPAS_Death) return;
	if (EquipmentItem->ActorHasTag("CloseRangedWeaponClass"))
	{
		if (CurrentSocketName == CloseRangedWeaponUnEquipSocketName)
		{
			SetWeaponToEquip_UnEquipSocketPoint(EquipmentItem, CloseRangedWeaponEquipSocketName);
		}
		else if (CurrentSocketName == CloseRangedWeaponEquipSocketName)
		{
			SetWeaponToEquip_UnEquipSocketPoint(EquipmentItem, CloseRangedWeaponUnEquipSocketName);
		}
	}
}

void APlayerClass::SetWeaponToEquip_UnEquipSocketPoint(AEquipableItemClass* EquipmentItem, FName SocketName)
{
	if (ActionState == EPlayerActionState::EPAS_Death) return;
	EquipmentItem->AttachMeshtoSocket(GetMesh(), SocketName);
}

FName APlayerClass::GetCurrentEquippedSocketName(AEquipableItemClass* EquipmentItem)
{
	if (ActionState == EPlayerActionState::EPAS_Death) return FName("");
	if (!IsValid(EquipmentItem)) return FName("");
	return EquipmentItem->GetAttachParentSocketName();
}

void APlayerClass::EquipCloseRangeWeaponProcess()
{
	if (ActionState == EPlayerActionState::EPAS_Death) return;
	if (!IsValid(EquipMotionMontage)) return;
	PlayAnimMontage(EquipMotionMontage);
	ActionState = EPlayerActionState::EPAS_Equipping;
}

void APlayerClass::UnEquipCloseRangeWeaponProcess()
{
	if (ActionState == EPlayerActionState::EPAS_Death) return;
	if (!IsValid(UnEquipMotionMontage)) return;
	PlayAnimMontage(UnEquipMotionMontage);
	ActionState = EPlayerActionState::EPAS_Equipping;
}

void APlayerClass::Initialize()
{
	Super::Initialize();

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	MoveComponent->bUseSeparateBrakingFriction = true;
	MoveComponent->bOrientRotationToMovement = true;

	MoveComponent->MaxWalkSpeed = 200.0f;
	MoveComponent->MinAnalogWalkSpeed = 20.0f;
	MoveComponent->BrakingDecelerationWalking = 2000.0f;

	MoveComponent->JumpZVelocity = 420.0f;
	MoveComponent->BrakingDecelerationFalling = 1500.0f;
	MoveComponent->AirControl = 0.35f;

	MoveComponent->RotationRate = FRotator{ 0.0f, 500.0f, 0.0f };

	MoveComponent->SetFixedBrakingDistance(200.0f);
	MoveComponent->NavAgentProps.AgentRadius = 42.0f;
	MoveComponent->NavAgentProps.AgentHeight = 192.0f;

	Tags.Add("Player");
}

void APlayerClass::CreateInitComponent()
{
	Super::CreateInitComponent();
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->bEnableCameraLag = true;

	FollowingCam = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowingCam"));
	FollowingCam->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowingCam->bUsePawnControlRotation = false;
}

void APlayerClass::CreateAppearanceComponent()
{
	Hair = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Hair"));
	Hair->SetupAttachment(GetMesh());
	Hair->LeaderPoseComponent = GetMesh();

	Scabbard = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Scabbard"));
	Scabbard->SetupAttachment(GetMesh());
	Scabbard->LeaderPoseComponent = GetMesh();
	Scabbard->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("Scabbard"));
}

void APlayerClass::CreateCombatComponent()
{
	Super::CreateCombatComponent();
	LockonComponent = CreateDefaultSubobject<ULockonComponent>(TEXT("Lockon"));
	ActionComponent = CreateDefaultSubobject<UActionComponent>(TEXT("Action"));
	ProjectileSpawnLocation = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSpawnPos"));
	ProjectileSpawnLocation->SetupAttachment(RootComponent);
}


void APlayerClass::StoreWeaponItem(AActor* ItemToStore)
{
	if (ActionState == EPlayerActionState::EPAS_Death) return;
	if (ItemToStore->ActorHasTag("CloseRangedWeaponClass"))
	{
		CloseRangedWeaponItem = Cast<ACloseRangedWeaponClass>(ItemToStore);
		CurrentEquippedWeapon = CloseRangedWeaponItem;
		CurrentEquippedWeapon->DisableNiagaraComponent();
		AnimInstance->CurrentEquippedWeapon = CurrentEquippedWeapon;
	}
	CurrentEquippedWeapon->SetSphereColliderCollision(ECollisionEnabled::NoCollision);
	CurrentEquippedWeapon->SetItemState(EItemState::EIS_Equipped);
	CurrentEquippedWeapon->AttachMeshtoSocket(GetMesh(), CloseRangedWeaponUnEquipSocketName);
}
