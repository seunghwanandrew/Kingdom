#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TraceComponent.generated.h"

class USkeletalMeshComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class KINGDOM_API UTraceComponent : public UActorComponent
{
	GENERATED_BODY()

#pragma region Variable
public:
protected:
private:
	/* Combat */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* SkeletalMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	AActor* Owner = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	FName Start;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	FName End;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	FName Rotation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	double BoxCollisionLength = 30.0f;
	UPROPERTY(VisibleAnywhere, Category = Combat, meta = (AllowPrivateAccess = "true"))
	bool bIsAttacking = false;
	UPROPERTY(VisibleInstanceOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	TArray<AActor*> TargetToIgnore;

	/* Debug */
	UPROPERTY(EditAnywhere, Category = Debug, meta = (AllowPrivateAccess = "true"))
	bool bDebugMode = false;
#pragma endregion

#pragma region Functions
public:	
	/* Setter */
	FORCEINLINE void SetTargetToIgnoreArrayEmpty() { TargetToIgnore.Empty(); }
	FORCEINLINE void IsAttacking(bool Value) { bIsAttacking = Value; }
	FORCEINLINE void SetOwner(AActor* NewOwner) { Owner = NewOwner; }

	UTraceComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;	
protected:
	virtual void BeginPlay() override;
private:
#pragma endregion		
};
