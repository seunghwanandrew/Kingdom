#include "FieldEffect/FireDamageEffector.h"
#include "Interfaces/HitInterface.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"

AFireDamageEffector::AFireDamageEffector()
{
	PrimaryActorTick.bCanEverTick = true;
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	SetRootComponent(BoxCollision);
	ParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystem"));
	ParticleSystem->SetupAttachment(RootComponent);
}

void AFireDamageEffector::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsOnEffectField && Target)
	{
		AActor* TargetRef = Cast<AActor>(Target);
		Target->GetEffectDamage_Implementation(Damage, nullptr, this, TargetRef->GetActorLocation());
	}
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireSound, GetActorLocation());
}

void AFireDamageEffector::BeginPlay()
{
	Super::BeginPlay();
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AFireDamageEffector::OnBoxCollisionBeginOverlap);
	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &AFireDamageEffector::OnBoxCollisionEndOverlap);
}

void AFireDamageEffector::OnBoxCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->Implements<UHitInterface>())
	{
		Target = Cast<IHitInterface>(OtherActor);
	}
	bIsOnEffectField = true;
}

void AFireDamageEffector::OnBoxCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Target = nullptr;
	bIsOnEffectField = false;
}
