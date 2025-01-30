#include "Items/BreakableItems/BreakableItemClass.h"
#include "GeometryCollection/GeometryCollectionComponent.h" 
#include "Field/FieldSystemComponent.h"
#include "Field/FieldSystemObjects.h"
#include "Components/CapsuleComponent.h"
#include "Items/PickableItems/PickableItemClass.h"
#include "Kismet/KismetMathLibrary.h"

ABreakableItemClass::ABreakableItemClass()
{
	PrimaryActorTick.bCanEverTick = true;

	CreateCapsuleCollisionComponentAsRootComponent();
	GeometryComponent = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("GeomeotryComponent"));
	GeometryComponent->SetupAttachment(RootComponent);
	GeometryComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CapsuleCollisionComponent->SetSimulatePhysics(false);
	CapsuleCollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CapsuleCollisionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);
	CapsuleCollisionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	CapsuleCollisionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Block);
	CapsuleCollisionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	CapsuleCollisionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Block);
	CapsuleCollisionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
	CapsuleCollisionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Block);
	CapsuleCollisionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Vehicle, ECollisionResponse::ECR_Block);
	CapsuleCollisionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Destructible, ECollisionResponse::ECR_Ignore);
	CapsuleCollisionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3, ECollisionResponse::ECR_Ignore);
	GeometryComponent->SetCollisionObjectType(ECollisionChannel::ECC_Destructible);
	GeometryComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);
	GeometryComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GeometryComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3, ECollisionResponse::ECR_Ignore);

	FieldSystem = CreateDefaultSubobject<UFieldSystemComponent>(TEXT("FieldSystem"));
	FieldSystem->SetupAttachment(RootComponent);

	MetaData = CreateDefaultSubobject<UFieldSystemMetaDataFilter>(TEXT("FieldMetaData"));
	MetaData->NewMetaData();
	MetaData->SetMetaDataFilterType(EFieldFilterType::Field_Filter_Dynamic, EFieldObjectType::Field_Object_Destruction, EFieldPositionType::Field_Position_CenterOfMass);

	RadialFalloff = CreateDefaultSubobject<URadialFalloff>(TEXT("RadialFalloff"));
	RadialVector = CreateDefaultSubobject<URadialVector>(TEXT("RadialVector"));
}

void ABreakableItemClass::BeginPlay()
{
	Super::BeginPlay();
}

void ABreakableItemClass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABreakableItemClass::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void ABreakableItemClass::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnSphereEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

void ABreakableItemClass::GetHit_Implementation(float DamageAmount, AController* EventInstigator, AActor* DamageCauser, FVector ImpactPoint)
{
	CreateField_Implementation(ImpactPoint);

	float RandFloat = FMath::FRand();
	if (RandFloat <= RandomSpawnRate)
	{
		int32 RandomIndex = FMath::RandRange(0, PossibleDropItem.Num() - 1);
		FVector AdjustSpawnPos = FVector{ 0.0f, 0.0f, 150.0f };
		FTransform SpawnTransform = UKismetMathLibrary::MakeTransform(GetActorLocation() + AdjustSpawnPos, GetActorRotation());
		SpawnPickableItem(RandomIndex, SpawnTransform);
	}

	OnBreak();
}

void ABreakableItemClass::CreateField_Implementation(const FVector& FieldLocation)
{
	if (FieldSystem)
	{
		if (!IsValid(RadialFalloff) || !IsValid(RadialVector)) return;
		RadialFalloff = RadialFalloff->SetRadialFalloff(MagnitudeValue, MinRangeValue, MaxRangeValue, DefaultValue, RadiusValue, FieldLocation, FalloffType);
		FieldSystem->UFieldSystemComponent::ApplyPhysicsField(true, EFieldPhysicsType::Field_ExternalClusterStrain, nullptr, RadialFalloff);
		RadialVector = RadialVector->SetRadialVector(MagnitudeValue + MagnitudeValue * 0.02f, FieldLocation);
		FieldSystem->UFieldSystemComponent::ApplyPhysicsField(true, EFieldPhysicsType::Field_LinearForce, MetaData, RadialVector);
	}
}

void ABreakableItemClass::DestroyField()
{
	if (FieldSystem)
	{
		RadialFalloff = nullptr;
		FieldSystem = nullptr;
	}
}

void ABreakableItemClass::OnBreak()
{
	DestroyField();
	GeometryComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	GeometryComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3, ECollisionResponse::ECR_Ignore);
	GeometryComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	SetLifeSpan(2.0f);
}

void ABreakableItemClass::SpawnPickableItem(int32 RandomIndex, FTransform& SpawnTransform)
{
	APickableItemClass* DropItem = GetWorld()->SpawnActorDeferred<APickableItemClass>(
		PossibleDropItem[RandomIndex], SpawnTransform,
		nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn
	);

	if (DropItem)
	{
		DropItem->SetItemState(EItemState::EIS_Rotating);
		DropItem->FinishSpawning(SpawnTransform);
		DropItem->AbleNiagraComponent();
	}
}
