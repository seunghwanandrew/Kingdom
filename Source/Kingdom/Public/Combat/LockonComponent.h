#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LockonComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(FOnUpdateTargetSignature, ULockonComponent, OnUpdateTargetDelegate, AActor*, TargetActor);

class UCharacterMovementComponent;
class USpringArmComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class KINGDOM_API ULockonComponent : public UActorComponent
{
	GENERATED_BODY()

#pragma region Variable
public:
	AActor* CurrentTargetActor;

	UPROPERTY(BlueprintAssignable)
	FOnUpdateTargetSignature OnUpdateTargetDelegate;
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat)
	ACharacter* Player;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat)
	APlayerController* PlayerController;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat)
	UCharacterMovementComponent* MoveComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat)
	USpringArmComponent* SpringArmComponent;
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	float TraceSphereRadius = 750.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	float BreakDistance = 1000.0f;
	
#pragma endregion
#pragma region Functions
public:	
	ULockonComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void StartLockon();
	UFUNCTION(BlueprintCallable)
	void EndLockon();
protected:
	virtual void BeginPlay() override;
private:
	bool TraceProcess_FindTargetActor(FHitResult OutResult);
#pragma endregion
};
