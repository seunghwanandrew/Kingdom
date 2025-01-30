#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterClass.h"
#include "Interfaces/PlayerInterface.h"
#include "Interfaces/HitInterface.h"
#include "EnumClass/EnumClass.h"
#include "GameFramework/SpringArmComponent.h"
#include "PlayerClass.generated.h"

class UActionComponent;
class UCameraComponent;
class UCapsuleComponent;
class ACloseRangedWeaponClass;
class ALongRangedWeaponClass;

class AEquipableItemClass;
class UInputMappingContext;
class UInputAction;

class UPlayerAnimInstance;
class USpringArmComponent;
class USkeletalMeshComponent;

class UTraceComponent;

struct FInputActionValue;


UCLASS()
class KINGDOM_API APlayerClass : public ACharacterClass, public IPlayerInterface
{
	GENERATED_BODY()
#pragma region Variables
public:
protected:
	UPROPERTY(VisibleAnywhere, Category = Items)
	AActor* OverlappedItem;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Items)
	AEquipableItemClass* CurrentEquippedWeapon;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Items)
	ACloseRangedWeaponClass* CloseRangedWeaponItem;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Items)
	TSubclassOf<AEquipableItemClass> LongRangedWeaponTemplate;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Items)
	ALongRangedWeaponClass* LongRangedWeaponItem;

	/* Widget UI */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
	TSubclassOf<UUserWidget> CrosshairWidgetBlueprint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
	TSubclassOf<UUserWidget> OnDeathWidget;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UI)
	TSubclassOf<UUserWidget> OnPauseWidget;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UI)
	UUserWidget* Crosshairs;

	UUserWidget* PauseWidget;

private:
	/* Input Component */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LockonAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* AttackAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* EKeyPressedAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* AimAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* CrouchAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SprintModeControlKey;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* RollAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ESCAction;

	/* Camera Component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"));
	USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"));
	UCameraComponent* FollowingCam;
	bool bIsPaused = false;

	/* Skeletal Mesh Component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Appearance, meta = (AllowPrivateAccess = "true"));
	USkeletalMeshComponent* Hair;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Appearance, meta = (AllowPrivateAccess = "true"));
	USkeletalMeshComponent* Scabbard;

	/* Action Component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"));
	UActionComponent* ActionComponent;

	/* Combat Variable */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = State, meta = (AllowPrivateAccess = "true"))
	EPlayerActionState ActionState = EPlayerActionState::EPAS_None;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = State, meta = (AllowPrivateAccess = "true"))
	bool bIsEngaging = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = State, meta = (AllowPrivateAccess = "true"))
	bool bIsAiming = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	FName CloseRangedWeaponEquipSocketName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	FName CloseRangedWeaponUnEquipSocketName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	FName LongRangedWeaponEquipSocketName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	FName LongRangedWeaponUnEquipSocketName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	FVector CamBoomZoomLocation{ 320.0f, 60.0f, 0.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	FVector CamBoomInitLocation{ 0.0f, 0.0f, 0.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	FVector TargetSocketOffset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	bool bIsMovingSocketOffset = false;

	/* Anim Instance */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"));
	UPlayerAnimInstance* AnimInstance;

	/* Animation_Montage */
	UPROPERTY(EditAnywhere, Category = Montage, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* EquipMotionMontage;
	UPROPERTY(EditAnywhere, Category = Montage, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* UnEquipMotionMontage;
	UPROPERTY(EditAnywhere, Category = Montage, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* BowEquipMotionMontage;
	UPROPERTY(EditAnywhere, Category = Montage, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* BowUnEquipMotionMontage;
	UPROPERTY(EditAnywhere, Category = Montage, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* AimingMotionMontage;
	UPROPERTY(EditAnywhere, Category = Montage, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* AimCancleMotionMontage;

#pragma endregion
#pragma region Functions
public:
	/* Setter */
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetActionState(const EPlayerActionState& StateCondition) { ActionState = StateCondition; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetIsEngaging(bool Value) { bIsEngaging = Value; }

	/* Getter */
	UFUNCTION(BlueprintCallable)
	FORCEINLINE EPlayerActionState GetActionState() { return ActionState; }
	FORCEINLINE bool GetbIsAiming() { return bIsAiming; }
	FORCEINLINE FVector GetCameraBoomOffset() { return CameraBoom->SocketOffset; }

	/* Event Handler */
	UFUNCTION(BlueprintAuthorityOnly)
	void OnUpdateTargetHandler(AActor* TargetActor);
	UFUNCTION(BlueprintAuthorityOnly)
	void OnAttackPerformedHandler(float AttackStamina);
	UFUNCTION(BlueprintAuthorityOnly)
	void OnMovementStaminaHandler(float StaminaUsage);
	UFUNCTION(BlueprintAuthorityOnly)
	void OnDeath(int32 Index);

	APlayerClass();
	virtual void Tick(float DeltaTime) override;
	virtual void NotifyControllerChanged() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	void SetOverlappedItem_Implementation(AActor* OverlappedItemRef);
	void HandleAttackEnd();
	UTraceComponent* GetCurrentEquipWeaponTraceComponent();
	void UnEquipPoint();
	AActor* GetCurrentEquippedWeapon();

	/* Interface */
	virtual void GetHit_Implementation(float DamageAmount, AController* EventInstigator, AActor* DamageCauser, FVector ImpactPoint) override;
	void GetEffectDamage_Implementation(float DamageAmount, AController* EventInstigator, AActor* DamageCauser, FVector ImpactPoint);
	bool HasEnoughStamina_Implementation(float MinValue);
	void TargetDeathProcess_Implementation();
	void StatsReCharge_Implementation(float HealthReChargeValue);
	void SetPlayerSoulOrItemAmount_Implementation(int32 SoulAmount, int32 ItemAmount);
	EPlayerActionState GetPlayerActionState_Implementation();

protected:
	virtual void BeginPlay() override;
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	virtual void Jump() override;
	virtual void StopJumping() override;
	void EKeyPressed();
	void LockonProcess();
	void ComboAttackProcess();
	void StealthMovement();
	void StealthToWalkMovement();
	void SprintModeControl();
	void Rolling();
	void HandleAimAction();
	void BowEquipToAim();
	void AimingProcess();
	UFUNCTION(BlueprintAuthorityOnly)
	void AdjustCamBoomLocation(FVector LocationValue);
	void SmoothMovementOfCamBoom(float DeltaTime);
	void AimCancle();
	void CurrentEquipMakesNullptr();
	void ESCPressed();

	UFUNCTION(BlueprintCallable, Category = Menu)
	void SetGamePause();
	UFUNCTION(BlueprintCallable, Category = Menu)
	void SetGameUnPause();

	UFUNCTION(BlueprintCallable)
	void SwitchingWeaponEquipSocket(AEquipableItemClass* EquipmentItem, FName CurrentSocketName);
	void SetWeaponToEquip_UnEquipSocketPoint(AEquipableItemClass* EquipmentItem, FName SocketName);
	UFUNCTION(BlueprintCallable)
	FName GetCurrentEquippedSocketName(AEquipableItemClass* EquipmentItem);

private:
	/* Montage */
	void EquipCloseRangeWeaponProcess();
	void UnEquipCloseRangeWeaponProcess();

	virtual void Initialize() override;
	virtual void CreateInitComponent() override;
	virtual void CreateCombatComponent() override;
	void CreateAppearanceComponent();
	void CreateCrosshairsWidget();
	void StoreWeaponItem(AActor* ItemToStore);
	void LongRangedWeaponSetupToPlayer();
#pragma endregion	

};
