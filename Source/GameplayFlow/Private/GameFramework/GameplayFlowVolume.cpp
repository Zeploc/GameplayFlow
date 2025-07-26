// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/GameplayFlowVolume.h"
#include "GameFramework/GameplayFlowDistributor.h"

#include "Components/BrushComponent.h"
#include "GameFramework/TriggerFlowDistributor.h"


AGameplayFlowVolume::AGameplayFlowVolume()
{
	PrimaryActorTick.bCanEverTick = false;

	FlowDistributorComponent = CreateDefaultSubobject<UTriggerFlowDistributor>("Flow Distributor");
}

void AGameplayFlowVolume::BeginPlay()
{
	Super::BeginPlay();

	UBrushComponent* TriggerBrushComponent = GetBrushComponent();
	if (!ensure(TriggerBrushComponent))
	{
		return;
	}
	FlowDistributorComponent->SetTriggerComponent(TriggerBrushComponent);
}
