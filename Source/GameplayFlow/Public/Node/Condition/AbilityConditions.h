// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ConditionFlowNode.h"
#include "GameplayEffectTypes.h"
#include "AbilityConditions.generated.h"

class UGameplayActorType;
	
/**
 * 
 */
UCLASS(abstract)
class GAMEPLAYFLOW_API UGameplayFlowAbilityCondition : public UGameplayFlowCondition
{
	GENERATED_BODY()

public:	
	/* Actor target to query */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced)
	TObjectPtr<UGameplayActorType> ActorTarget;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText OverrideDisplayName;
	
	UFUNCTION(BlueprintPure)
	AActor* GetTargetActor() const;

protected:
	virtual void ExecuteInput(const FName& PinName) override;
	virtual void Cleanup() override;
	virtual void BindActorEvents();

	UFUNCTION(BlueprintImplementableEvent, DisplayName="Bind Actor Events")
	void K2_BindActorEvents();
	/* Called when the actor is removed or the event condition is cleaned up */
	virtual void CleanUpActorEvents();
	virtual bool EvaluatePredicate_Implementation() const override;
	
#if WITH_EDITORONLY_DATA
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;	
	virtual FText GetNodeTitle() const override;
#endif

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
};

UCLASS(abstract)
class GAMEPLAYFLOW_API UGameplayFlowTagsCondition : public UGameplayFlowAbilityCondition
{
	GENERATED_BODY()

protected:
	virtual void BindActorEvents() override;
	virtual void CleanUpActorEvents() override;
	
	virtual FGameplayTagContainer GetAllConditionalTags() const { return {}; }

	UFUNCTION()
	void OnTagsChanged(const FGameplayTag Tag, int32 Count);
};

UCLASS(DisplayName="Tag Criteria")
class GAMEPLAYFLOW_API UGameplayFlowTagCriteriaCondition : public UGameplayFlowTagsCondition
{
	GENERATED_BODY()

public:	
	/* Tag criteria for actor */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Actor", meta = (DisplayAfter="ActorTarget"))
	FGameplayTagRequirements TagRequirements;

protected:
	
	virtual bool EvaluatePredicate_Implementation() const override;
	virtual FGameplayTagContainer GetAllConditionalTags() const override;
};

UCLASS(DisplayName="Tag Count")
class GAMEPLAYFLOW_API UGameplayFlowTagCountCondition : public UGameplayFlowTagsCondition
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Actor", meta = (DisplayAfter="ActorTarget"))
	FGameplayTagContainer Tags;

	/* At least this many count of the tag required */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Actor", meta = (DisplayAfter="ActorTarget"))
	int32 RequiredTagCount = 1;

	/* Requires exact matching count to pass condition */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Actor", meta = (DisplayAfter="ActorTarget"))
	bool bExactOnly = false;

protected:
	
	virtual bool EvaluatePredicate_Implementation() const override;
	virtual FGameplayTagContainer GetAllConditionalTags() const override;
};
