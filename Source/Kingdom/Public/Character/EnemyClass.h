// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/EnemyInterface.h"
#include "Interfaces/HitInterface.h"
#include "EnemyClass.generated.h"

class UStatsComponent;

class UTargetDisplayerWidgetComponent;

UCLASS()
class KINGDOM_API AEnemyClass : public ACharacter, public IEnemyInterface, public IHitInterface
{
	GENERATED_BODY()

#pragma region Variables
public:
protected:
private:
	/* Combat Component */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UStatsComponent* StatsComponent;

	/* User Widget Component */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UTargetDisplayerWidgetComponent* TargetDisplayer;
#pragma endregion

#pragma region Functions
public:
	AEnemyClass();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void GetHit_Implementation(float DamageAmount, AController* EventInstigator, AActor* DamageCauser);
protected:
	virtual void BeginPlay() override;

	void OnTargeted_Implementation() override;
	void UnTargeted_Implementation() override;

private:
	void Initialize();
	void CreateInitComponent();
	void CreateCombatComponent();
#pragma endregion
};
