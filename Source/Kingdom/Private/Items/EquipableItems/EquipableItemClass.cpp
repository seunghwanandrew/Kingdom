#include "Items/EquipableItems/EquipableItemClass.h"
#include "Interfaces/PlayerInterface.h"
#include "Components/SphereComponent.h"
#include "Combat/TraceComponent.h"
#include "NiagaraFunctionLibrary.h"

AEquipableItemClass::AEquipableItemClass()
{
	CreateSkeletalMeshComponentAsRootComponent();
	CreateNiagaraComponent();
	ItemState = EItemState::EIS_Hovering;
}

void AEquipableItemClass::Set_EmptyTraceTargetToIgnoreVariable()
{
	TraceComponent->SetTargetToIgnoreArrayEmpty();
}

void AEquipableItemClass::SpawnPickupEmitter()
{
	if (!IsValid(PickupEffect)) return;
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), PickupEffect, GetActorLocation());
}

void AEquipableItemClass::BeginPlay()
{
	Super::BeginPlay();

}

void AEquipableItemClass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEquipableItemClass::AttachMeshtoSocket(USceneComponent* Parent, const FName& SocketName)
{
	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
	SkeletalMesh->AttachToComponent(Parent, TransformRules, SocketName);
}

void AEquipableItemClass::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

}
void AEquipableItemClass::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnSphereEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

void AEquipableItemClass::HandleOverlappedItemEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::HandleOverlappedItemEvent(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	if (OtherActor)
	{
		if (!OtherActor->Implements<UPlayerInterface>()) return;
		IPlayerInterface::Execute_SetOverlappedItem(OtherActor, this);
	}
}

void AEquipableItemClass::HandleEndOverlappedItemEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::HandleEndOverlappedItemEvent(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	if (!OtherActor->Implements<UPlayerInterface>()) return;
	IPlayerInterface::Execute_SetOverlappedItem(OtherActor, nullptr);
}



