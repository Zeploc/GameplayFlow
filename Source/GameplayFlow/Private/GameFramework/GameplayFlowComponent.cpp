// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/GameplayFlowComponent.h"


UGameplayFlowComponent::UGameplayFlowComponent()
{
	
}

void UGameplayFlowComponent::SetFlowInstigator(const TObjectPtr<AActor>& NewFlowInstigator)
{
	FlowInstigator = NewFlowInstigator;
}

void UGameplayFlowComponent::BeginPlay()
{
	if (!FlowInstigator)
	{
		// Default to owner, set before blueprint begin play
		FlowInstigator = GetOwner();
	}
	
	Super::BeginPlay();

	// TODO: Handle if flow started from elsewhere, grab that flow instance and trigger event
	// Further issues with this in relation to Owner/Instigator not being usable, see later paragraph in the Context section of the design doc
}

