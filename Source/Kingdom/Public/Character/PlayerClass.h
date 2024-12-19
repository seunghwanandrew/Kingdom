#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/PlayerInterface.h"
#include "Interfaces/HitInterface.h"
#include "EnumClass/EnumClass.h"
#include "PlayerClass.generated.h"

class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;
class USkeletalMeshComponent;
class UCapsuleComponent;
class ULockonComponent;
class UCombatComponent;
class UTraceComponent;
class UPlayerAnimInstance;
struct FInputActionValue;

class AEquipableItemClass;
class ACloseRangedWeaponClass;

UCLASS()
class KINGDOM_API APlayerClass : public ACharacter, public IPlayerInterface, public IHitInterface
{
	GENERATED_BODY()
#pragma region Variable
public:
protected:
	UPROPERTY(VisibleAnywhere, Category = Items)
	AActor* OverlappedItem;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Items)
	AEquipableItemClass* CurrentEquippedWeapon;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Items)
	ACloseRangedWeaponClass* CloseRangedWeaponItem;
private:
	/* Other Component */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UCharacterMovementComponent* MoveComponent;

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

	/* Combat Component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"));
	ULockonComponent* Lockon;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"));
	UCombatComponent* Combat;

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
	UAnimMontage* EquipProcessMontage;
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
	FORCEINLINE virtual float GetDamage() override { return 15.0f; }
	float Implementation_GetDamage() { return 20.0f; }

	/* Getter */
	UFUNCTION(BlueprintCallable)
	FORCEINLINE UCharacterMovementComponent* GetMoveComponent() { return MoveComponent; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE EPlayerActionState GetActionState() { return ActionState; }

	APlayerClass();
	virtual void Tick(float DeltaTime) override;
	virtual void NotifyControllerChanged() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintAuthorityOnly)
	void OnUpdateTargetHandler(AActor* TargetActor);
	void SetOverlappedItem_Implementation(AActor* OverlappedItemRef);
	void HandleAttackEnd();
	UTraceComponent* GetCurrentEquipWeaponTraceComponent();
	void UnEquipPoint();
protected:
	virtual void BeginPlay() override;
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void EKeyPressed();
	void LockonProcess();
	void ComboAttackProcess();

	UFUNCTION(BlueprintCallable)
	void SwitchingWeaponEquipSocket(AEquipableItemClass* EquipmentItem, FName CurrentSocketName);
	void SetWeaponToEquip_UnEquipSocketPoint(AEquipableItemClass* EquipmentItem, FName SocketName);
	UFUNCTION(BlueprintCallable)
	FName GetCurrentEquippedSocketName(AEquipableItemClass* EquipmentItem);

private:
	/* Montage */
	void EquipCloseRangeWeaponProcess();
	void UnEquipCloseRangeWeaponProcess();

	void Initialize();
	void CreateInitComponent();
	void CreateAppearanceComponent();
	void CreateCombatComponent();	
	void StoreWeaponItem(AActor* ItemToStore);
#pragma endregion	

};
