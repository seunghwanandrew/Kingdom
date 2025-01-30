// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "EnumClass/EnumClass.h"
#include "CharacterClass.generated.h"

class UCombatComponent;
class ULockonComponent;
class UStatsComponent;

UCLASS()
class KINGDOM_API ACharacterClass : public ACharacter, public IHitInterface
{
	GENERATED_BODY()
#pragma region Variables
public:
protected:
	/* Other Component */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Component)
	UCharacterMovementComponent* MoveComponent;

	/* Combat Component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat)
	ULockonComponent* LockonComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat)
	UCombatComponent* CombatComponent;

	/* Stats Component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat)
	UStatsComponent* StatsComponent;

	/* Montage */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Montage)
	TMap<EHitFrom, UAnimMontage*> HitReactionList;

	/* Effect : Particle */
	UPROPERTY(EditAnywhere, Category = VFX)
	UParticleSystem* HitBloodEffect;
	UPROPERTY(EditAnywhere, Category = VFX)
	UParticleSystem* FireDamageEffect;
private:

#pragma endregion

#pragma region Functions
public:
	/* Getter */
	UFUNCTION(BlueprintCallable)
	FORCEINLINE UCharacterMovementComponent* GetMoveComponent() { return MoveComponent; }
	FORCEINLINE UStatsComponent* GetStatsComponent() { return StatsComponent; }
	FORCEINLINE UCombatComponent* GetCombatComponent() { return CombatComponent; }

	ACharacterClass();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

	virtual void Initialize();
	virtual void CreateInitComponent();
	virtual void CreateCombatComponent();
	void HitReactionMontagePlay(FVector ImpactPoint);

	/* Interface */
	float GetCharacterStrength_Implementation();
	virtual void GetHit_Implementation(float DamageAmount, AController* EventInstigator, AActor* DamageCauser, FVector ImpactPoint) override;
private:
	UPROPERTY(EditAnywhere, Category = Debug)
	bool bIsDebugMode = false;
#pragma endregion

};
