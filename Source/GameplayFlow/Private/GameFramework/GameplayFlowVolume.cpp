// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/GameplayFlowVolume.h"
#include "GameFramework/GameplayFlowDistributor.h"

#include "Components/BrushComponent.h"


AGameplayFlowVolume::AGameplayFlowVolume()
{
	PrimaryActorTick.bCanEverTick = false;

	FlowDistributorComponent = CreateDefaultSubobject<UGameplayFlowDistributor>("Flow Distributor");
}

void AGameplayFlowVolume::BeginPlay()
{
	Super::BeginPlay();

	UBrushComponent* TriggerBrushComponent = GetBrushComponent();
	if (!ensure(TriggerBrushComponent))
	{
		return;
	}
	TriggerBrushComponent->OnComponentBeginOverlap.AddUniqueDynamic(this, &AGameplayFlowVolume::ActorBeginOverlap);
	TriggerBrushComponent->OnComponentEndOverlap.AddUniqueDynamic(this, &AGameplayFlowVolume::ActorEndOverlap);
}

void AGameplayFlowVolume::ActorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	FlowDistributorComponent->DistributeFlow(OtherActor);
}

void AGameplayFlowVolume::ActorEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	FlowDistributorComponent->SendEvent(OtherActor, EndTriggerEventName);
}
