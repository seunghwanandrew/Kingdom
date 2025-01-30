#include "Items/PickableItems/PickableItemClass.h"
#include "Components/SphereComponent.h"
#include "Interfaces/PlayerInterface.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

APickableItemClass::APickableItemClass()
{
	PrimaryActorTick.bCanEverTick = true;
	CreateStaticMeshComponentAsRootComponent();
	CreateNiagaraComponent();
	StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SphereCollisionComponent->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel3);
	SphereCollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereCollisionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);
	SphereCollisionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	SphereCollisionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Destructible, ECollisionResponse::ECR_Ignore);
	SphereCollisionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Ignore);
	SphereCollisionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3, ECollisionResponse::ECR_Ignore);
	NiagaraComponent->SetVisibility(false);
}

void APickableItemClass::BeginPlay()
{
	Super::BeginPlay();

	GetObjectSurfaceZLocation();
}

void APickableItemClass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ItemSpawningDown(DeltaTime);
}

void APickableItemClass::GetObjectSurfaceZLocation()
{
	FHitResult HitResult;

	FVector Start = GetActorLocation();
	FVector End = Start - FVector{ 0.0f, 0.0f, 2000.f };

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldStatic));

	TArray<AActor*> Ignores;
	Ignores.Add(this);

	EDrawDebugTrace::Type Trace = bIsDebugMode ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;
	UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), Start, End, ObjectTypes, true, Ignores, Trace, HitResult, true);

	if (HitResult.bBlockingHit)
	{
		DesiredTargetLocationZValue = (HitResult.ImpactPoint + TargetHeight).Z;
	}
}

void APickableItemClass::ItemSpawningDown(float DeltaTime)
{
	if (GetActorLocation().Z > DesiredTargetLocationZValue)
	{
		FVector MoveRate = FVector{ 0.0f, 0.0f, -20.0f } *DeltaTime;
		AddActorWorldOffset(MoveRate);
	}
}

void APickableItemClass::HandleOverlappedItemEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::HandleOverlappedItemEvent(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	if (!IsValid(OtherActor)) return;

	bool bIsPlayerRef = OtherActor->Implements<UPlayerInterface>() ? true : false;

	if (bIsPlayerRef)
	{
		IPlayerInterface* PlayerInterface = Cast<IPlayerInterface>(OtherActor);
		PlayerInterface->SetPlayerSoulOrItemAmount_Implementation(Soul, Amount);
	}

	if (PickupEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), PickupEffect, GetActorLocation());
	}

	Destroy();
}
