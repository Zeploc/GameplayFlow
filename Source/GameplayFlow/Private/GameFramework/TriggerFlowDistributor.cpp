// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/TriggerFlowDistributor.h"


UTriggerFlowDistributor::UTriggerFlowDistributor()
{
}

void UTriggerFlowDistributor::BeginPlay()
{
	Super::BeginPlay();

	UPrimitiveComponent* FirstTriggerComponent = GetOwner()->GetComponentByClass<UPrimitiveComponent>();
	if (!FirstTriggerComponent)
	{
		AddTriggerComponent(FirstTriggerComponent);
	}	
}

void UTriggerFlowDistributor::ActorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	DistributeFlow(OtherActor);
}

void UTriggerFlowDistributor::ActorEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	SendEvent(OtherActor, EndTriggerEventName);
}

void UTriggerFlowDistributor::AddTriggerComponent(UPrimitiveComponent* TriggerComponent)
{
	bool bIsAlreadyInSet;
	TriggerComponents.Add(TriggerComponent, &bIsAlreadyInSet);
	if (!bIsAlreadyInSet)
	{
		TriggerComponent->OnComponentBeginOverlap.AddUniqueDynamic(this, &UTriggerFlowDistributor::ActorBeginOverlap);
		TriggerComponent->OnComponentEndOverlap.AddUniqueDynamic(this, &UTriggerFlowDistributor::ActorEndOverlap);
	}
}

void UTriggerFlowDistributor::RemoveTriggerComponent(UPrimitiveComponent* TriggerComponent)
{
	if (TriggerComponents.Remove(TriggerComponent) > 0)
	{
		TriggerComponent->OnComponentBeginOverlap.RemoveDynamic(this, &UTriggerFlowDistributor::ActorBeginOverlap);
		TriggerComponent->OnComponentEndOverlap.RemoveDynamic(this, &UTriggerFlowDistributor::ActorEndOverlap);
	}
}

void UTriggerFlowDistributor::SetTriggerComponent(UPrimitiveComponent* TriggerComponent)
{
	const TSet<UPrimitiveComponent*> RemoveTriggerComponents = TriggerComponents;
	for (UPrimitiveComponent* TriggerComponentToRemove : RemoveTriggerComponents)
	{
		RemoveTriggerComponent(TriggerComponentToRemove);
	}
	AddTriggerComponent(TriggerComponent);
}

