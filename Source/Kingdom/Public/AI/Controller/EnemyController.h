#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyController.generated.h"

class AEnemyClass;
class UBehaviorTree;
class UBlackboardComponent;
class UCharacterMovementComponent;

UCLASS()
class KINGDOM_API AEnemyController : public AAIController
{
	GENERATED_BODY()

#pragma region Variables
public:

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EnemyRef)
	AEnemyClass* EnemyRef;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Behavior")
	UBehaviorTree* EnemyBehavior;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Behavior")
	UBlackboardComponent* EnemyBlackboard;
	UPROPERTY(VisibleAnywhere, Category = "AI Behavior")
	AActor* CombatTarget;
	UPROPERTY(VisibleAnywhere, Category = "AI Behavior")
	UCharacterMovementComponent* MoveComponent;
	UPROPERTY(VisibleAnywhere, Category = "AI Behavior")
	bool bIsTarget = false;
private:
	
#pragma endregion

#pragma region Functions
public:
	/* Getter */
	FORCEINLINE AActor* GetTargetActor() { return CombatTarget; }
	virtual void Tick(float DeltaTime) override;
	/* Event Handler */
	UFUNCTION(BlueprintAuthorityOnly)
	void OnTargetDetected(AActor* Target, bool Value);
protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
private:

#pragma endregion
};
