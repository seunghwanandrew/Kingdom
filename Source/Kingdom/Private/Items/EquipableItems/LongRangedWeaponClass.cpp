#include "Items/EquipableItems/LongRangedWeaponClass.h"
#include "Components/SphereComponent.h"

ALongRangedWeaponClass::ALongRangedWeaponClass()
{
	Tags.Add("LongRangedWeaponClass");
}

void ALongRangedWeaponClass::BeginPlay()
{
	Super::BeginPlay();
	SphereCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ALongRangedWeaponClass::OnSphereOverlap);
	SphereCollisionComponent->OnComponentEndOverlap.AddDynamic(this, &ALongRangedWeaponClass::OnSphereEndOverlap);
}

void ALongRangedWeaponClass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALongRangedWeaponClass::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void ALongRangedWeaponClass::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnSphereEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

void ALongRangedWeaponClass::HandleOverlappedItemEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::HandleOverlappedItemEvent(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void ALongRangedWeaponClass::HandleEndOverlappedItemEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::HandleEndOverlappedItemEvent(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}
