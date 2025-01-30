#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FireDamageEffector.generated.h"

class IHitInterface;
class UBoxComponent;

UCLASS()
class KINGDOM_API AFireDamageEffector : public AActor
{
	GENERATED_BODY()
	
#pragma region Variables
public:
	IHitInterface* Target;
protected:
	UPROPERTY(EditAnywhere)
	UBoxComponent* BoxCollision;
	UParticleSystemComponent* ParticleSystem;
	UPROPERTY(EditAnywhere)
	USoundBase* FireSound;

private:
	UPROPERTY(EditAnywhere, Category = Damage)
	float Damage = 0.3f;
	UPROPERTY(EditAnywhere, Category = Damage)
	bool bIsOnEffectField = false;
#pragma endregion
#pragma region Functions
public:	
	AFireDamageEffector();
	virtual void Tick(float DeltaTime) override;
protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintAuthorityOnly)
	void OnBoxCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION(BlueprintAuthorityOnly)
	void OnBoxCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
private:	
#pragma endregion
};
