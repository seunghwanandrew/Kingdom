// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/EnemyInterface.h"
#include "EnemyClass.generated.h"

class UTargetDisplayerWidgetComponent;

UCLASS()
class KINGDOM_API AEnemyClass : public ACharacter, public IEnemyInterface
{
	GENERATED_BODY()

#pragma region Variable
public:
protected:
private:
	/* User Widget Component */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"));
	UTargetDisplayerWidgetComponent* TargetDisplayer;
#pragma endregion

#pragma region Functions
public:
	AEnemyClass();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
protected:
	virtual void BeginPlay() override;

	void OnTargeted_Implementation() override;
	void UnTargeted_Implementation() override;
private:
	void Initialize();
	void CreateInitComponent();
#pragma endregion
};
