// Fill out your copyright notice in the Description page of Project Settings.


#include "Node/Condition/ConditionFlowNode.h"

#include "AddOns/FlowNodeAddOn_PredicateAND.h"


void UGameplayFlowCondition::Cleanup()
{
	Super::Cleanup();
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
}

UConditionGameplayFlowNode::UConditionGameplayFlowNode()
{	
	OutputPins.Empty();
	OutputPins.Add(FFlowPin(ConditionsMetPinName));
	OutputPins.Add(FFlowPin(ConditionsUnmetPinName));
	OutputPins.Add(FFlowPin(ConditionsInitialUnmetPinName));
#if WITH_EDITOR
	Category = TEXT("Gameplay Flow");
	NodeStyle = EFlowNodeStyle::Latent;
#endif
}

void UConditionGameplayFlowNode::ExecuteInput(const FName& PinName)
{
	Super::ExecuteInput(PinName);

	for (UFlowNodeAddOn* const& AddOn : AddOns)
	{
		UGameplayFlowCondition* GameplayFlowCondition = Cast<UGameplayFlowCondition>(AddOn);
		if (!GameplayFlowCondition)
		{
			continue;
		}
		Conditions.Add(GameplayFlowCondition);
	}
	for (TObjectPtr<UGameplayFlowCondition>& GameplayFlowCondition : Conditions)
	{
		if (!GameplayFlowCondition)
		{
			continue;
		}
		GameplayFlowCondition->OnEventConditionChanged.AddDynamic(this, &UConditionGameplayFlowNode::OnEventConditionChanged);
	}
	UpdateConditionState();
}

EFlowAddOnAcceptResult UConditionGameplayFlowNode::AcceptFlowNodeAddOnChild_Implementation(const UFlowNodeAddOn* AddOnTemplate) const
{
	if (IFlowPredicateInterface::ImplementsInterfaceSafe(AddOnTemplate))
	{
		return EFlowAddOnAcceptResult::TentativeAccept;
	}
	return Super::AcceptFlowNodeAddOnChild_Implementation(AddOnTemplate);
}

void UConditionGameplayFlowNode::OnEventConditionChanged(UGameplayFlowCondition* EventCondition)
{
	UpdateConditionState();
}

void UConditionGameplayFlowNode::Cleanup()
{
	Super::Cleanup();
#if WITH_EDITOR
	TriggeredCount = 0;
#endif
}

void UConditionGameplayFlowNode::UpdateConditionState()
{
	if (GetActivationState() != EFlowNodeState::Active)
	{
		return;
	}
	
	bool bNewConditionMet = true;
	for (int Index = 0; Index < AddOns.Num(); ++Index)
	{
		const UFlowNodeAddOn* AddOn = AddOns[Index];

		if (IFlowPredicateInterface::ImplementsInterfaceSafe(AddOn))
		{
			bNewConditionMet = bNewConditionMet && IFlowPredicateInterface::Execute_EvaluatePredicate(AddOn);
		}
	}
	
	if (bConditionsMet == bNewConditionMet)
	{
		return;
	}
	bConditionsMet = bNewConditionMet;
#if WITH_EDITOR
	if (bConditionsMet)
	{
		TriggeredCount++;
	}
#endif
	// Only finish if trigger once, otherwise should continue to trigger
	const bool bFinish = bConditionsMet && bTriggerOnce;
	TriggerOutput(bConditionsMet ? ConditionsMetPinName : ConditionsUnmetPinName, bFinish);
}

#if WITH_EDITOR
FString UConditionGameplayFlowNode::GetNodeDescription() const
{
	return bTriggerOnce ? TEXT("Trigger Once") : TEXT("Repeat Trigger");
	// return Conditions.IsEmpty() ? TEXT("No Conditions") : FString::Printf(TEXT("%i Conditions"), Conditions.Num());
}

FString UConditionGameplayFlowNode::GetStatusString() const
{
	if (GetActivationState() != EFlowNodeState::Active)
	{
		return FString();
	}
	
	FString StatusString;
	for (const TObjectPtr<UGameplayFlowCondition>& FlowCondition : Conditions)
	{
		if (!FlowCondition)
		{
			continue;
		}
		FString CanExecute = IFlowPredicateInterface::Execute_EvaluatePredicate(FlowCondition) ? TEXT("True") : TEXT("False");
		StatusString += TEXT("[") + FString::FormatAsNumber(Conditions.IndexOfByKey(FlowCondition)) + TEXT("] ") + FlowCondition->GetName() + TEXT(": ") + CanExecute;
	}

	return StatusString;
}

void UConditionGameplayFlowNode::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	OutputPins.Empty();
	FFlowPin TruePin(ConditionsMetPinName);
	FFlowPin FalsePin(ConditionsUnmetPinName);
	TruePin.PinFriendlyName = FText::FromString(TEXT("Triggered True"));
	FalsePin.PinFriendlyName = FText::FromString(TEXT("Triggered False"));
	OutputPins.Add(TruePin);
	OutputPins.Add(FalsePin);
	if (bCanFireFalseOnEnter)
	{
		// FalsePin.PinFriendlyName = FText::FromString(TEXT("False"));
		
		FFlowPin InitialFalsePin(ConditionsInitialUnmetPinName);
		InitialFalsePin.PinFriendlyName = FText::FromString(TEXT("Trigger Initial False"));
		if (!bCanTriggerFalseOnEnter)
		{
			InitialFalsePin.PinFriendlyName = FText::FromString(TEXT("Initially False"));
		}
		if ((!bCanTriggerFalseOnEnter) || bSplitInitialFalseOutput)
		{
			OutputPins.Add(InitialFalsePin);
		}
	}
	OnReconstructionRequested.ExecuteIfBound();
}
#endif
