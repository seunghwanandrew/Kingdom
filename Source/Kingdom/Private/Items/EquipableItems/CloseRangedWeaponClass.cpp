#include "Items/EquipableItems/CloseRangedWeaponClass.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Combat/TraceComponent.h"

ACloseRangedWeaponClass::ACloseRangedWeaponClass()
{
	Tags.Add("CloseRangedWeaponClass");
	TraceComponent = CreateDefaultSubobject<UTraceComponent>(TEXT("Trace"));
}

void ACloseRangedWeaponClass::BeginPlay()
{
	Super::BeginPlay();
	SphereCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ACloseRangedWeaponClass::OnSphereOverlap);
	SphereCollisionComponent->OnComponentEndOverlap.AddDynamic(this, &ACloseRangedWeaponClass::OnSphereEndOverlap);
}

void ACloseRangedWeaponClass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACloseRangedWeaponClass::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void ACloseRangedWeaponClass::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnSphereEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

void ACloseRangedWeaponClass::HandleOverlappedItemEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::HandleOverlappedItemEvent(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	TraceComponent->SetOwner(OtherActor);
}

void ACloseRangedWeaponClass::HandleEndOverlappedItemEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::HandleEndOverlappedItemEvent(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}
