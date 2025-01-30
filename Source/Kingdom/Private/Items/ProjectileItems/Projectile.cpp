#include "Items/ProjectileItems/Projectile.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Character/PlayerClass.h"
#include "Interfaces/HitInterface.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>("Default");
	SetRootComponent(DefaultSceneRoot);
	Projectile = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileObject"));
	Projectile->SetupAttachment(RootComponent);
	NiagaraEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara Effect"));
	NiagaraEffect->SetupAttachment(RootComponent);
	ParticleEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle Effect"));
	ParticleEffect->SetupAttachment(RootComponent);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->bRotationFollowsVelocity = true;

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereCollision->SetSphereRadius(24.0f);
	SphereCollision->SetupAttachment(RootComponent);

	Tags.Add(FName("Projectile"));
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	this->SetHidden(true);
}

void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AProjectile::AddDelegate()
{
	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnSphereBeginOverlap);
}

void AProjectile::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag(FName("Projectile")) || OtherActor->ActorHasTag(FName("Player"))) return;
	ProjectileMovement->StopMovementImmediately();
	SetZeroGravity();

	AttachToActor(OtherActor, FAttachmentTransformRules::KeepWorldTransform);
	SphereCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ParticleEffect->SetVisibility(false);
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, SphereCollision->GetComponentLocation());

	float Damage = 0.0f;

	if (GetOwner() && GetOwner()->Implements<UHitInterface>())
	{
		Damage = GetOwner<IHitInterface>()->Execute_GetCharacterStrength(GetOwner()) * MultipleArrowPower;
	}	

	if (OtherActor->Implements<UHitInterface>())
	{
		IHitInterface* HitActor = Cast<IHitInterface>(OtherActor);
		if (HitActor->IsBoss_Implementation())
		{
			Damage *= 0.05f;
		}
		HitActor->GetHit_Implementation(Damage, GetInstigator()->GetController(), this, SphereCollision->GetComponentLocation());
	}

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), GetArrowHitSound, SweepResult.ImpactPoint);
	SetLifeSpan(2.0f);
}

void AProjectile::SetVelocity(FVector Vector)
{
	ProjectileMovement->Velocity = Vector;
}

void AProjectile::SetZeroGravity()
{
	ProjectileMovement->ProjectileGravityScale = 0.0f;
}

void AProjectile::SetProjectileSpeed_Gravity(float InitSpeed, float MaxSpeed, float GravityValue)
{
	ProjectileMovement->InitialSpeed = InitSpeed;
	ProjectileMovement->MaxSpeed = MaxSpeed;
	ProjectileMovement->ProjectileGravityScale = GravityValue;
}