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
	EnemyRef->OnTargetDectectedDelegate.AddDynamic(this, &AEnemyController::OnTargetDetected);
	MoveComponent = EnemyRef->GetCharacterMovement();
}

void AEnemyController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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
}
