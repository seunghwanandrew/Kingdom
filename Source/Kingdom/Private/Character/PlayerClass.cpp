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
#include "GameFramework/Actor.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Items/EquipableItems/CloseRangedWeaponClass.h"
#include "Items/EquipableItems/LongRangedWeaponClass.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DamageEvents.h"

APlayerClass::APlayerClass()
{
	PrimaryActorTick.bCanEverTick = true;
	Initialize();	
	CreateInitComponent();
	CreateAppearanceComponent();
	CreateCombatComponent();

	Tags.Add(FName("Player"));
}

void APlayerClass::BeginPlay()
{
	Super::BeginPlay();

	LongRangedWeaponSetupToPlayer();

	CreateCrosshairsWidget();
	
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

	SmoothMovementOfCamBoom(DeltaTime);
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
	EnhancedInput->BindAction(AimAction, ETriggerEvent::Triggered, this, &APlayerClass::HandleAimAction);
	EnhancedInput->BindAction(CrouchAction, ETriggerEvent::Triggered, this, &APlayerClass::StealthMovement);
	EnhancedInput->BindAction(CrouchAction, ETriggerEvent::Completed, this, &APlayerClass::StealthToWalkMovement);
	EnhancedInput->BindAction(SprintModeControlKey, ETriggerEvent::Triggered, this, &APlayerClass::SprintModeControl);
	EnhancedInput->BindAction(RollAction, ETriggerEvent::Triggered, this, &APlayerClass::Rolling);
	EnhancedInput->BindAction(ESCAction, ETriggerEvent::Triggered, this, &APlayerClass::ESCPressed);
}

void APlayerClass::OnUpdateTargetHandler(AActor* TargetActor)
{
	if (TargetActor)
	{
		if (CurrentEquippedWeapon == nullptr || CurrentEquippedWeapon == LongRangedWeaponItem) return;
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
	CreateWidget(GetWorld(), OnDeathWidget, FName("Death Menu"))->AddToViewport();

	FInputModeGameOnly UIOnlyMode;
	GetWorld()->GetFirstPlayerController()->SetInputMode(UIOnlyMode);
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
}

void APlayerClass::SetOverlappedItem_Implementation(AActor* OverlappedItemRef)
{
	OverlappedItem = OverlappedItemRef;
}

void APlayerClass::HandleAttackEnd()
{
	SetActionState(EPlayerActionState::EPAS_Combat);
	AnimInstance->SetActionState(EPlayerActionState::EPAS_Combat);
	if (CurrentEquippedWeapon->ActorHasTag(FName("CloseRangedWeaponClass")))
	{
		CurrentEquippedWeapon->Set_EmptyTraceTargetToIgnoreVariable();
	}
}

UTraceComponent* APlayerClass::GetCurrentEquipWeaponTraceComponent()
{
	if (!(CurrentEquippedWeapon == CloseRangedWeaponItem)) return nullptr;
	return CurrentEquippedWeapon->GetTraceComponent();

}

void APlayerClass::UnEquipPoint()
{
	FName CurrentWeaponEquipPointSocketName = GetCurrentEquippedSocketName(CurrentEquippedWeapon);
	SwitchingWeaponEquipSocket(CurrentEquippedWeapon, CurrentWeaponEquipPointSocketName);
}

AActor* APlayerClass::GetCurrentEquippedWeapon()
{
	if (!IsValid(CurrentEquippedWeapon)) return nullptr;
	return Cast<AActor>(CurrentEquippedWeapon);
}

void APlayerClass::GetHit_Implementation(float DamageAmount, AController* EventInstigator, AActor* DamageCauser, FVector ImpactPoint)
{
	if (ActionComponent->IsRollingMode()) return;

	Super::GetHit_Implementation(DamageAmount, EventInstigator, DamageCauser, ImpactPoint);
	if (!bIsEngaging && DamageCauser->ActorHasTag("Boss"))
	{
		if (bIsAiming)
		{
			HandleAimAction();
		}
		LockonComponent->StartLockon(DamageCauser);
	}
	if (ActionState == EPlayerActionState::EPAS_Attacking)
	{
		ActionState = EPlayerActionState::EPAS_Combat;
	}
}

void APlayerClass::GetEffectDamage_Implementation(float DamageAmount, AController* EventInstigator, AActor* DamageCauser, FVector ImpactPoint)
{
	HitReactionMontagePlay(ImpactPoint);
	FDamageEvent TargetAttackedEvent;
	float Damage = TakeDamage(DamageAmount, TargetAttackedEvent, EventInstigator, DamageCauser);

	StatsComponent->ApplyDamage(Damage);
}

bool APlayerClass::HasEnoughStamina_Implementation(float MinValue)
{
	return StatsComponent->Stats[EStats::ES_Stamina] >= MinValue;
}

void APlayerClass::TargetDeathProcess_Implementation()
{
	LockonComponent->EndLockon();
}

void APlayerClass::StatsReCharge_Implementation(float HealthReChargeValue)
{
	if (HealthReChargeValue > 0.0f)
	{
		StatsComponent->StartHealthReGen(HealthReChargeValue);
	}
}

void APlayerClass::SetPlayerSoulOrItemAmount_Implementation(int32 SoulAmount, int32 ItemAmount)
{
	CombatComponent->SetCurrentHoldArrowAmount(ItemAmount);
	CombatComponent->SetCurrentSoulAmount(SoulAmount);
}

EPlayerActionState APlayerClass::GetPlayerActionState_Implementation()
{
	return ActionState;
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
	else if (IsValid(CloseRangedWeaponItem))
	{
		if (ActionState <= EPlayerActionState::EPAS_Attacking || bIsEngaging) return;

		if (bIsAiming)
		{
			HandleAimAction();
		}

		if (CurrentEquippedWeapon != CloseRangedWeaponItem || CurrentEquippedWeapon == nullptr)
		{
			CurrentEquippedWeapon = CloseRangedWeaponItem;
		}
		
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
	if (CurrentEquippedWeapon == CloseRangedWeaponItem)
	{
		ActionState = EPlayerActionState::EPAS_Attacking;
		CombatComponent->Player_ComboAttack();
	}
	else if (CurrentEquippedWeapon == LongRangedWeaponItem)
	{
		int32 RemainArrows = CombatComponent->GetCurrentHoldArrowAmount();
		if (RemainArrows > 0)
		{
			ActionState = EPlayerActionState::EPAS_Attacking;
			CombatComponent->Player_ProjectileEjection();
		}
		else
		{
			HandleAimAction();
		}
	}
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

void APlayerClass::HandleAimAction()
{
	if (bIsAiming)
	{
		AimCancle();
	}
	else
	{
		BowEquipToAim();
	}
}

void APlayerClass::BowEquipToAim()
{
	if (!IsValid(LongRangedWeaponItem) || ActionState < EPlayerActionState::EPAS_None) return;
	
	CurrentEquippedWeapon = LongRangedWeaponItem;
	ActionState = EPlayerActionState::EPAS_Equipping;
	float PlayTime = PlayAnimMontage(BowEquipMotionMontage);
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &APlayerClass::AimingProcess, PlayTime);
}

void APlayerClass::AimingProcess()
{
	ActionComponent->AimWalkMovement();
	float PlayTime = PlayAnimMontage(AimingMotionMontage);
	FTimerHandle TimerHandle;
	FTimerDelegate AdjustCamBoom;
	AdjustCamBoom.BindUFunction(this, FName("AdjustCamBoomLocation"), CamBoomZoomLocation);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, AdjustCamBoom, PlayTime, false);
	bIsAiming = true;
	AnimInstance->SetIsAiming(bIsAiming);
	MoveComponent->bOrientRotationToMovement = false;
	MoveComponent->bUseControllerDesiredRotation = true;
	bUseControllerRotationYaw = true;
}

void APlayerClass::AdjustCamBoomLocation(FVector LocationValue)
{
	if (bIsAiming)
	{
		Crosshairs->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		Crosshairs->SetVisibility(ESlateVisibility::Hidden);
		float PlayTime = PlayTime = PlayAnimMontage(BowUnEquipMotionMontage);
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &APlayerClass::CurrentEquipMakesNullptr, PlayTime);
	}

	TargetSocketOffset = LocationValue;
	bIsMovingSocketOffset = true;
}

void APlayerClass::SmoothMovementOfCamBoom(float DeltaTime)
{
	if (!bIsMovingSocketOffset) return;

	FVector CurrentOffset = CameraBoom->SocketOffset;
	FVector NewOffset = FMath::VInterpTo(CurrentOffset, TargetSocketOffset, DeltaTime, 3.0f);
	CameraBoom->SocketOffset = NewOffset;

	if (CameraBoom->SocketOffset.Equals(TargetSocketOffset, 0.01f))
	{
		bIsMovingSocketOffset = false;
	}
}

void APlayerClass::AimCancle()
{
	ActionState = EPlayerActionState::EPAS_Equipping;
	ActionComponent->BasicWalkMovement();
	float PlayTime = PlayAnimMontage(AimCancleMotionMontage);
	FTimerHandle TimerHandle;
	FTimerDelegate AdjustCamBoom;
	AdjustCamBoom.BindUFunction(this, FName("AdjustCamBoomLocation"), CamBoomInitLocation);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, AdjustCamBoom, PlayTime, false);
	bIsAiming = false;
	AnimInstance->SetIsAiming(bIsAiming);
	MoveComponent->bOrientRotationToMovement = true;
	MoveComponent->bUseControllerDesiredRotation = false;
	bUseControllerRotationYaw = false;	
}

void APlayerClass::CurrentEquipMakesNullptr()
{
	CurrentEquippedWeapon = nullptr;
}

void APlayerClass::ESCPressed()
{
	if (!IsValid(PauseWidget)) PauseWidget = CreateWidget(GetWorld(), OnPauseWidget, FName("Pause Menu"));

	if (!bIsPaused)
	{
		SetGamePause();
	}
	else
	{
		SetGameUnPause();
	}
}

void APlayerClass::SetGamePause()
{
	UGameplayStatics::SetGamePaused(this, true);
	PauseWidget->AddToViewport();

	FInputModeGameOnly GameAndUIMode;
	GetWorld()->GetFirstPlayerController()->SetInputMode(GameAndUIMode);
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
}

void APlayerClass::SetGameUnPause()
{
	UGameplayStatics::SetGamePaused(this, false);
	PauseWidget->RemoveFromParent();
	FInputModeGameOnly GameOnlyMode;
	GetWorld()->GetFirstPlayerController()->SetInputMode(GameOnlyMode);
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = false;
	bIsPaused = false;
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
	else if (EquipmentItem->ActorHasTag("LongRangedWeaponClass"))
	{
		if (CurrentSocketName == LongRangedWeaponUnEquipSocketName)
		{
			SetWeaponToEquip_UnEquipSocketPoint(EquipmentItem, LongRangedWeaponEquipSocketName);
		}
		else if (CurrentSocketName == LongRangedWeaponEquipSocketName)
		{
			SetWeaponToEquip_UnEquipSocketPoint(EquipmentItem, LongRangedWeaponUnEquipSocketName);
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
	CameraBoom->SetRelativeLocation(CamBoomInitLocation);

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

void APlayerClass::CreateCrosshairsWidget()
{
	if (!IsValid(CrosshairWidgetBlueprint)) return;
	Crosshairs = CreateWidget(Cast<APlayerController>(GetController()), CrosshairWidgetBlueprint, FName("Crosshairs"));
	Crosshairs->AddToViewport();
	Crosshairs->SetVisibility(ESlateVisibility::Hidden);
}

void APlayerClass::CreateCombatComponent()
{
	Super::CreateCombatComponent();
	LockonComponent = CreateDefaultSubobject<ULockonComponent>(TEXT("Lockon"));
	ActionComponent = CreateDefaultSubobject<UActionComponent>(TEXT("Action"));
}


void APlayerClass::StoreWeaponItem(AActor* ItemToStore)
{
	if (ActionState == EPlayerActionState::EPAS_Death) return;
	if (!(ItemToStore->ActorHasTag("CloseRangedWeaponClass"))) return;

	CloseRangedWeaponItem = Cast<ACloseRangedWeaponClass>(ItemToStore);
	CloseRangedWeaponItem->DisableNiagaraComponent();
	CloseRangedWeaponItem->SpawnPickupEmitter();
	CloseRangedWeaponItem->SetSphereColliderCollision(ECollisionEnabled::NoCollision);
	CloseRangedWeaponItem->SetItemState(EItemState::EIS_Equipped);
	CloseRangedWeaponItem->AttachMeshtoSocket(GetMesh(), CloseRangedWeaponUnEquipSocketName);
}

void APlayerClass::LongRangedWeaponSetupToPlayer()
{
	if (IsValid(LongRangedWeaponTemplate))
	{
		LongRangedWeaponItem = GetWorld()->SpawnActor<ALongRangedWeaponClass>(LongRangedWeaponTemplate);
		LongRangedWeaponItem->AttachMeshtoSocket(GetMesh(), LongRangedWeaponUnEquipSocketName);
	}
}
