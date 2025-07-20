// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AddOns/FlowNodeAddOn.h"
#include "Interfaces/FlowPredicateInterface.h"
#include "Nodes/FlowNode.h"
#include "ConditionFlowNode.generated.h"

UCLASS(Abstract, Blueprintable, EditInlineNew, CollapseCategories)
class GAMEPLAYFLOW_API UGameplayFlowCondition
	: public UFlowNodeAddOn
	, public IFlowPredicateInterface
{
	GENERATED_BODY()
	
public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEventConditionChanged, UGameplayFlowCondition*, EventCondition);
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnEventConditionChanged OnEventConditionChanged;
	
	// virtual void Register() { }
	virtual void Cleanup() override;
};

/**
 * Handles latent conditional checks, which will recheck the conditions when a state changes and fire off once the condtions are met
 */
UCLASS(NotBlueprintable, Category="Gameplay Flow", meta=(DisplayName="Latent Condition"))
class GAMEPLAYFLOW_API UConditionGameplayFlowNode : public UFlowNode
{
	GENERATED_BODY()
	
public:
	UConditionGameplayFlowNode();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Debug")
	TArray<TObjectPtr<UGameplayFlowCondition>> Conditions;
	
#if WITH_EDITORONLY_DATA
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Debug")
	int32 TriggeredCount = 0;
#endif
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bTriggerOnce = true;

	// Note: Not currently hooked up, still in design
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bCanFireFalseOnEnter = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(EditCondition="bCanFireFalseOnEnter", EditConditionHides=true))
	bool bCanTriggerFalseOnEnter = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(EditCondition="bCanFireFalseOnEnter && bCanTriggerFalseOnEnter", EditConditionHides=true))
	bool bSplitInitialFalseOutput = false;

	virtual EFlowAddOnAcceptResult AcceptFlowNodeAddOnChild_Implementation(const UFlowNodeAddOn* AddOnTemplate) const override;

protected:
	UFUNCTION()
	void OnEventConditionChanged(UGameplayFlowCondition* EventCondition);
	virtual void ExecuteInput(const FName& PinName) override;
	virtual void Cleanup() override;
	
	void UpdateConditionState();
	
#if WITH_EDITOR
	virtual FString GetNodeDescription() const override;
	virtual FString GetStatusString() const override;

	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	bool bConditionsMet = false;
	static inline const FName ConditionsMetPinName = TEXT("True");
	static inline const FName ConditionsUnmetPinName = TEXT("On False");
	static inline const FName ConditionsInitialUnmetPinName = TEXT("Initially False");
};


