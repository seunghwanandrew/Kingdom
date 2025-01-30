#include "Items/PickableItems/StatsPortionClass.h"
#include "Components/SphereComponent.h"
#include "Interfaces/PlayerInterface.h"

AStatsPortionClass::AStatsPortionClass()
{
	PrimaryActorTick.bCanEverTick = true;
	SphereCollisionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);
	SphereCollisionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	SphereCollisionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Ignore);
	SphereCollisionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Destructible, ECollisionResponse::ECR_Ignore);
}

void AStatsPortionClass::BeginPlay()
{
	Super::BeginPlay();
}

void AStatsPortionClass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AStatsPortionClass::HandleOverlappedItemEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsValid(OtherActor)) return;

	bool bIsPlayer = OtherActor->Implements<UPlayerInterface>() ? true : false;

	if (bIsPlayer)
	{
		IPlayerInterface* PlayerRef = Cast<IPlayerInterface>(OtherActor);
		PlayerRef->StatsReCharge_Implementation(HealthRechargeAmount);
	}

	Super::HandleOverlappedItemEvent(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}
