#include "Items/BaseItemClass.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"

ABaseItemClass::ABaseItemClass()
{
	PrimaryActorTick.bCanEverTick = true;	
}

void ABaseItemClass::BeginPlay()
{
	Super::BeginPlay();
	if (SphereCollisionComponent)
	{
		SphereCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ABaseItemClass::OnSphereOverlap);
		SphereCollisionComponent->OnComponentEndOverlap.AddDynamic(this, &ABaseItemClass::OnSphereEndOverlap);
		SphereCollisionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Ignore);
	}
}

void ABaseItemClass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (ItemState == EItemState::EIS_Hovering)
	{
		TransformedPosition(DeltaTime);
	}
	if (ItemState == EItemState::EIS_Rotating)
	{
		TransformedRotator(DeltaTime);
	}

	if (ItemState == EItemState::EIS_Hovering || ItemState == EItemState::EIS_Rotating)
	{
		if (!SphereCollisionComponent) return;
		SphereCollisionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Ignore);
		SphereCollisionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Ignore);
	}
	else
	{
		if (!SphereCollisionComponent) return;
		SphereCollisionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
		SphereCollisionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Overlap);
	}
}

void ABaseItemClass::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	HandleOverlappedItemEvent(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void ABaseItemClass::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	HandleEndOverlappedItemEvent(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

void ABaseItemClass::TransformedPosition(float DeltaTime)
{
	RunningTime += DeltaTime;
	float DeltaZ = Amplitude * FMath::Sin(RunningTime * TimeConstant);
	AddActorWorldOffset(FVector(0.0f, 0.0f, DeltaZ));
}

void ABaseItemClass::TransformedRotator(float DeltaTime)
{
	float RotationZ = (DeltaTime *= RotationSpeed);
	AddActorWorldRotation(FQuat(FRotator(0.0f, RotationZ, 0.0f)));
}

void ABaseItemClass::CreateSkeletalMeshComponentAsRootComponent()
{
	if (!IsValid(SkeletalMesh))
	{
		SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	}
	SkeletalMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetRootComponent(SkeletalMesh);
	CreateSphereCollisionComponent();
}

void ABaseItemClass::CreateStaticMeshComponentAsRootComponent()
{
	if (!IsValid(StaticMesh))
	{
		StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	}
	StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetRootComponent(StaticMesh);
	CreateSphereCollisionComponent();
}

void ABaseItemClass::CreateCapsuleCollisionComponentAsRootComponent()
{
	if (!IsValid(CapsuleCollisionComponent))
	{
		CapsuleCollisionComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleCollision"));
	}
	SetRootComponent(CapsuleCollisionComponent);
	CapsuleCollisionComponent->SetGenerateOverlapEvents(true);
}

void ABaseItemClass::CreateSphereCollisionComponentAsRootComponent()
{
	if (!IsValid(SphereCollisionComponent))
	{
		SphereCollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	}
	SetRootComponent(SphereCollisionComponent);
	SphereCollisionComponent->SetGenerateOverlapEvents(true);
}

void ABaseItemClass::CreateSphereCollisionComponent()
{
	if (!IsValid(SphereCollisionComponent))
	{
		SphereCollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	}
	SphereCollisionComponent->SetupAttachment(RootComponent);
	SphereCollisionComponent->SetGenerateOverlapEvents(true);
}

void ABaseItemClass::CreateNiagaraComponent()
{
	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara Effect"));
	NiagaraComponent->SetupAttachment(RootComponent);
}

void ABaseItemClass::SetSphereColliderCollision(ECollisionEnabled::Type Condition)
{
	SphereCollisionComponent->SetCollisionEnabled(Condition);
	SphereCollisionComponent->SetVisibility(false);
}

void ABaseItemClass::HandleOverlappedItemEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void ABaseItemClass::HandleEndOverlappedItemEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}
