// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/GameplayFlowDistributor.h"
#include "GameFramework/GameplayFlowComponent.h"

#include "FlowAsset.h"
#include "GameplayFlowLogs.h"


UGameplayFlowDistributor::UGameplayFlowDistributor()
{
	PrimaryComponentTick.bCanEverTick = false;
	FlowComponentClass = UGameplayFlowComponent::StaticClass();
}

void UGameplayFlowDistributor::DistributeFlow(AActor* TargetActor)
{
	if (!FlowAsset)
	{
		return;
	}
	if (!FlowComponentClass)
	{
		FlowComponentClass = UGameplayFlowComponent::StaticClass();
	}
	CleanupInactiveFlows(TargetActor);
	if (FlowComponents.Contains(TargetActor) && !bMultipleDistributionInstancesPerTarget)
	{
		const FName MultipleDistributionVarName = GET_MEMBER_NAME_CHECKED(ThisClass, bMultipleDistributionInstancesPerTarget);
		GAMEPLAY_FLOW_LOG_WARNING("Attempted to distribute multiple flows to %s via %s when %s is disabled! Did you finish your flow?", *TargetActor->GetName(), *GetOwner()->GetName(), *MultipleDistributionVarName.ToString())
		return;
	}
	if (HasMatchingFlowAsset(TargetActor) && bSkipExistingMatchingFlowAsset)
	{
		return;
	}
	const FString ComponentName = FString::Printf(TEXT("%s_%s_%i"), *GetOwner()->GetName(), *FlowAsset->GetName(), FlowComponents.FindOrAdd(TargetActor).Num());	
	UGameplayFlowComponent* GameplayFlowComponent = NewObject<UGameplayFlowComponent>(TargetActor, FlowComponentClass, FName(ComponentName));
	GameplayFlowComponent->RootFlow = FlowAsset;
	GameplayFlowComponent->SetFlowInstigator(GetOwner());
	GameplayFlowComponent->RegisterComponent();
	FlowComponents.FindOrAdd(TargetActor).Add(GameplayFlowComponent);
	// TODO: Bind event to when the flow is finished to remove the component
}

bool UGameplayFlowDistributor::SendEvent(AActor* TargetActor, const FName EventName)
{
	if (!FlowComponents.Contains(TargetActor))
	{
		return false;
	}

	bool bSentEvent = false;
	const TArray<UGameplayFlowComponent*>& GameplayFlowComponents = FlowComponents[TargetActor];
	for (UGameplayFlowComponent* GameplayFlowComponent : GameplayFlowComponents)
	{
		if (!GameplayFlowComponent || !GameplayFlowComponent->GetRootFlowInstance())
		{
			continue;
		}
		GameplayFlowComponent->GetRootFlowInstance()->TriggerCustomInput(EventName);
		bSentEvent = true;
	}
	return bSentEvent;
}

void UGameplayFlowDistributor::RemoveFlow(AActor* TargetActor)
{
	if (!FlowComponents.Contains(TargetActor))
	{
		return;
	}
	const TArray<UGameplayFlowComponent*>& GameplayFlowComponents = FlowComponents[TargetActor];
	for (UGameplayFlowComponent* GameplayFlowComponent : GameplayFlowComponents)
	{
		GameplayFlowComponent->DestroyComponent();
	}
	FlowComponents.Remove(TargetActor);
}

bool UGameplayFlowDistributor::HasMatchingFlowAsset(const AActor* TargetActor) const
{
	TArray<UGameplayFlowComponent*> FoundFlowComponents;
	TargetActor->GetComponents<UGameplayFlowComponent>(FoundFlowComponents);
	for (UGameplayFlowComponent* FlowComponent : FoundFlowComponents)
	{
		// If active flow with this asset running
		if (FlowComponent->RootFlow == FlowAsset && FlowComponent->GetRootFlowInstance())
		{
			return true;
		}
	}
	return false;
}

void UGameplayFlowDistributor::CleanupInactiveFlows(const AActor* TargetActor)
{
	if (!FlowComponents.Contains(TargetActor))
	{
		// No flows exist for actor, no action to be done
		return;
	}
	TArray<UGameplayFlowComponent*>& TargetFlowComponents = FlowComponents[TargetActor];
	// Copy so removal can occur
	const TArray<UGameplayFlowComponent*> GameplayFlowComponents = TargetFlowComponents;
	for (UGameplayFlowComponent* GameplayFlowComponent : GameplayFlowComponents)
	{
		// No flow instance running when it previously was
		if (GameplayFlowComponent && !GameplayFlowComponent->GetRootFlowInstance())
		{
			GameplayFlowComponent->DestroyComponent();
			TargetFlowComponents.Remove(GameplayFlowComponent);
		}
	}
	if (TargetFlowComponents.IsEmpty())
	{
		FlowComponents.Remove(TargetActor);
	}
}