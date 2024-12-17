#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/PlayerInterface.h"
#include "EnumClass/EnumClass.h"
#include "PlayerClass.generated.h"

class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;
class USkeletalMeshComponent;
class ULockonComponent;
class UPlayerAnimInstance;
struct FInputActionValue;

UCLASS()
class KINGDOM_API APlayerClass : public ACharacter, public IPlayerInterface
{
	GENERATED_BODY()
#pragma region Variable
public:
protected:
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

	/* Combat Variable */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = State, meta = (AllowPrivateAccess = "true"))
	EPlayerActionState CombatState = EPlayerActionState::EPAS_None;

	/* Anim Instance */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"));
	UPlayerAnimInstance* AnimInstance;

#pragma endregion
#pragma region Functions
public:
	/* Getter */
	FORCEINLINE UCharacterMovementComponent* GetMoveComponent() { return MoveComponent; }
	FORCEINLINE EPlayerActionState GetCombatState() { return CombatState; }

	APlayerClass();
	virtual void Tick(float DeltaTime) override;
	virtual void NotifyControllerChanged() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintAuthorityOnly)
	void OnUpdateTargetHandler(AActor* TargetActor);
protected:
	virtual void BeginPlay() override;
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void LockonProcess();
private:
	void Initialize();
	void CreateInitComponent();
	void CreateAppearanceComponent();
	void CreateCombatComponent();
#pragma endregion	

};
