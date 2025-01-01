#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterClass.h"
#include "Interfaces/PlayerInterface.h"
#include "Interfaces/HitInterface.h"
#include "EnumClass/EnumClass.h"
#include "PlayerClass.generated.h"

class UActionComponent;
class UCameraComponent;
class UCapsuleComponent;
class ACloseRangedWeaponClass;

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
	UInputAction* CrouchAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SprintModeControlKey;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* RollAction;

	/* Camera Component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"));
	USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"));
	UCameraComponent* FollowingCam;

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	FName CloseRangedWeaponEquipSocketName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	FName CloseRangedWeaponUnEquipSocketName;

	/* Anim Instance */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"));
	UPlayerAnimInstance* AnimInstance;

	/* Animation_Montage */
	UPROPERTY(EditAnywhere, Category = Montage, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* EquipMotionMontage;
	UPROPERTY(EditAnywhere, Category = Montage, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* UnEquipMotionMontage;

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

	/* Interface */
	virtual void GetHit_Implementation(float DamageAmount, AController* EventInstigator, AActor* DamageCauser, FVector ImpactPoint) override;
	bool HasEnoughStamina_Implementation(float MinValue);
	void TargetDeathProcess_Implementation();

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
	void StoreWeaponItem(AActor* ItemToStore);
#pragma endregion	

};
