#include "Character/CharacterClass.h"
#include "Combat/CombatComponent.h"
#include "Components/CapsuleComponent.h"
#include "Character/StatsComponent.h"
#include "Engine/DamageEvents.h"
#include "Interfaces/EnemyInterface.h"
#include "Interfaces/PlayerInterface.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

ACharacterClass::ACharacterClass()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACharacterClass::BeginPlay()
{
	Super::BeginPlay();	
}

void ACharacterClass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACharacterClass::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ACharacterClass::Initialize()
{
	MoveComponent = GetCharacterMovement();
	GetCapsuleComponent()->SetCapsuleSize(42.0f, 96.0f);
}

void ACharacterClass::CreateInitComponent()
{
}

void ACharacterClass::CreateCombatComponent()
{
	CombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
	StatsComponent = CreateDefaultSubobject<UStatsComponent>(TEXT("StatsComponent"));
}

void ACharacterClass::HitReactionMontagePlay(FVector ImpactPoint)
{
	ACharacter* CharacterRef = Cast<ACharacter>(this);

	FVector HitPoint = (ImpactPoint - CharacterRef->GetActorLocation()).GetSafeNormal();
	FVector Forward = CharacterRef->GetActorForwardVector();
	
	// Forward * HitPoint = |Forward||HitPoint| * Cos(Theta)
	// |Forward| == 1, |HitPoint| == 1, so Forward * HitPoint = Cos(Theta)
	double CosTheta = FVector::DotProduct(Forward, HitPoint);
	double Theta = FMath::RadiansToDegrees(FMath::Acos(CosTheta));

	// If Forward Cross HitPoint is positive CrossProduct(Plus Value) indicate up : it means Hit point is right
	// Otherwise CroosProduct(Minus Value) indicate down : it means Hit point is left 
	const FVector CrossProduct = FVector::CrossProduct(Forward, HitPoint);

	if (bIsDebugMode)
	{
		const FVector ImpactVerticalPoint = { ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z };
		FVector VerticalHitPoint = (ImpactVerticalPoint - CharacterRef->GetActorLocation()).GetSafeNormal();
		UKismetSystemLibrary::DrawDebugArrow(GetWorld(), GetActorLocation(), GetActorLocation() + Forward * 60.0f, 5.0f, FColor::Red, true);
		UKismetSystemLibrary::DrawDebugArrow(GetWorld(), GetActorLocation(), GetActorLocation() + VerticalHitPoint * 60.0f, 5.0f, FColor::Blue, true);
		UKismetSystemLibrary::DrawDebugArrow(GetWorld(), GetActorLocation(), GetActorLocation() + CrossProduct * 100.0f, 5.0f, FColor::Green, true);
	}	
	
	if (CrossProduct.Z < 0)
	{
		Theta *= -1.0f;
	}

	EHitFrom HitPointLocation = EHitFrom::EHF_Back;
	if (Theta)
	{
		if (Theta >= -45.0f && Theta < 45.0f) HitPointLocation = EHitFrom::EHF_Front;
		else if (Theta >= 45.0f && Theta < 135.0f) HitPointLocation = EHitFrom::EHF_Right;
		else if (Theta >= -135.0f && Theta < -45.0f) HitPointLocation = EHitFrom::EHF_Left;
	}
	CharacterRef->PlayAnimMontage(HitReactionList[HitPointLocation]);
}

float ACharacterClass::GetCharacterStrength_Implementation()
{
	return StatsComponent->Stats[EStats::ES_Strength];
}

void ACharacterClass::GetHit_Implementation(float DamageAmount, AController* EventInstigator, AActor* DamageCauser, FVector ImpactPoint)
{
	// To Do Hit Reaction;
	HitReactionMontagePlay(ImpactPoint);
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitBloodEffect, ImpactPoint);

	FDamageEvent TargetAttackedEvent;
	float Damage = TakeDamage(DamageAmount, TargetAttackedEvent, EventInstigator, DamageCauser);

	StatsComponent->ApplyDamage(Damage);
}

