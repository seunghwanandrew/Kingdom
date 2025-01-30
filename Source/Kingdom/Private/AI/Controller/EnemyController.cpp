#include "AI/Controller/EnemyController.h"
#include "Character/EnemyClass.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void AEnemyController::BeginPlay()
{
	Super::BeginPlay();
	EnemyRef = Cast<AEnemyClass>(GetPawn());
	if (!IsValid(EnemyRef)) return;
	EnemyRef->OnTargetDectectedDelegate.AddDynamic(this, &AEnemyController::OnTargetDetected);
	MoveComponent = EnemyRef->GetCharacterMovement();
}

void AEnemyController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!IsValid(EnemyRef)) return;
	if (bIsTarget)
	{
		MoveComponent->MaxWalkSpeed = EnemyRef->GetMaxWalkSpeedValue();
	}
	else
	{
		MoveComponent->MaxWalkSpeed = EnemyRef->GetInitWalkSpeedValue();
	}
}

void AEnemyController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	Cast<AAIController>(InPawn->GetController())->RunBehaviorTree(EnemyBehavior);
	EnemyBlackboard = GetBlackboardComponent();
}

void AEnemyController::OnTargetDetected(AActor* Target, bool Value)
{
	CombatTarget = Target;
	bIsTarget = Value;

	if (Blackboard)
	{
		Blackboard->SetValueAsObject(FName("Target"), CombatTarget);
		Blackboard->SetValueAsBool(FName("IsTarget"), bIsTarget);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(5, 3.0f, FColor::Red, TEXT("No Blackboard"));
	}
}
