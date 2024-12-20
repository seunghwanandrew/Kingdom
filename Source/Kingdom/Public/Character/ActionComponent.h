#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ActionComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(FOnStealthMovementSignature, UActionComponent, OnStealthMovementDelegate, float, StealthStaminaUsage);

class IPlayerInterface;
class UCharacterMovementComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class KINGDOM_API UActionComponent : public UActorComponent
{
	GENERATED_BODY()

#pragma region Variables
public:
	UPROPERTY(BlueprintAssignable)
	FOnStealthMovementSignature OnStealthMovementDelegate;
protected:
private:
	ACharacter* CharacterRef;
	IPlayerInterface* PlayerInterface;
	UCharacterMovementComponent* MoveComponent;

	UPROPERTY(EditAnywhere, Category = Movement, meta = (AllowPrivateAccess = "ture"))
	float StaminaToUseForStealthMove = 0.08f;
	UPROPERTY(EditAnywhere, Category = Movement, meta = (AllowPrivateAccess = "ture"))
	float StealthMoveSpeed = 200.0f ;
	UPROPERTY(EditAnywhere, Category = Movement, meta = (AllowPrivateAccess = "ture"))
	float WalkMoveSpeed = 600.0f;
#pragma endregion

#pragma region Functions
public:	
	UActionComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void BasicWalkMovement();
	UFUNCTION(BlueprintCallable)
	void StealthMovement();
protected:
	virtual void BeginPlay() override;

private:
#pragma endregion
		
};
