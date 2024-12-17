#include "Character/PlayerClass.h"

#include "Camera/CameraComponent.h"

#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"

#include "AnimInstances/PlayerAnimInstance.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

#include "Combat/LockonComponent.h"

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

	Lockon->OnUpdateTargetDelegate.AddDynamic(this, &APlayerClass::OnUpdateTargetHandler);
}

void APlayerClass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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

	EnhancedInput->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
	EnhancedInput->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
	EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerClass::Move);
	EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerClass::Look);
	EnhancedInput->BindAction(LockonAction, ETriggerEvent::Started, this, &APlayerClass::LockonProcess);
}

void APlayerClass::Move(const FInputActionValue& Value)
{
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

void APlayerClass::LockonProcess()
{
	if (CombatState == EPlayerActionState::EPAS_None)
	{
		Lockon->StartLockon();
	}
	else
	{
		Lockon->EndLockon();
	}
}

void APlayerClass::OnUpdateTargetHandler(AActor* TargetActor)
{
	if (TargetActor)
	{
		CombatState = EPlayerActionState::EPAS_Combat;
	}
	else
	{
		CombatState = EPlayerActionState::EPAS_None;
	}
	AnimInstance->OnUpdateTargetHandle();
	UE_LOG(LogTemp, Warning, TEXT("Target State - %s"), TargetActor ? TEXT("Target :"), *TargetActor->GetName() : TEXT("Target is nulltpr"));
}

void APlayerClass::Initialize()
{
	GetCapsuleComponent()->SetCapsuleSize(42.0f, 96.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	MoveComponent = GetCharacterMovement();

	MoveComponent->bUseSeparateBrakingFriction = true;
	MoveComponent->bOrientRotationToMovement = true;

	MoveComponent->MaxWalkSpeed = 600.0f;
	MoveComponent->MinAnalogWalkSpeed = 20.0f;
	MoveComponent->BrakingDecelerationWalking = 2000.0f;

	MoveComponent->JumpZVelocity = 700.0f;
	MoveComponent->BrakingDecelerationFalling = 1500.0f;
	MoveComponent->AirControl = 0.35f;

	MoveComponent->RotationRate = FRotator{ 0.0f, 500.0f, 0.0f };

	MoveComponent->SetFixedBrakingDistance(200.0f);
	MoveComponent->NavAgentProps.AgentRadius = 42.0f;
	MoveComponent->NavAgentProps.AgentHeight = 192.0f;
}

void APlayerClass::CreateInitComponent()
{
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
	Lockon = CreateDefaultSubobject<ULockonComponent>(TEXT("Lockon"));
}
