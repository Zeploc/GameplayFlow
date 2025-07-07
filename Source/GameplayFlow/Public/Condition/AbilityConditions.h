// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ConditionFlowNode.h"
#include "GameplayEffectTypes.h"
#include "AbilityConditions.generated.h"


UCLASS(Abstract, Blueprintable, EditInlineNew, CollapseCategories)
class GAMEPLAYFLOW_API UGameplayActorType : public UObject
{
	GENERATED_BODY()
	
public:

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnActorFound, UGameplayActorType*, ActorType, AActor*, FoundActor);
	FOnActorFound OnActorFound;
	
	FOnActorFound OnActorLost;

	virtual void Register();
	virtual AActor* GetActor() const;
	virtual void CleanUp();

protected:
	UPROPERTY()
	TObjectPtr<AActor> FoundLiveActor;
	
	void TryRegisterActor();
	virtual AActor* TryResolveActor();
	void OnActorRegistered();
	void UnregisterActor(AActor* Actor, bool bBroadcastLost);
	
	UFUNCTION()
	void BoundActorEndPlay(AActor* Actor , EEndPlayReason::Type EndPlayReason);

	// TODO:
	UFUNCTION()
	void ItemFound(AActor* RegisteredItem);
};

UCLASS(DisplayName="Player")
class GAMEPLAYFLOW_API UGameplayActorType_Player : public UGameplayActorType
{
	GENERATED_BODY()
	
protected:
	virtual AActor* TryResolveActor() override;	
};

UCLASS(DisplayName="Owner")
class GAMEPLAYFLOW_API UGameplayActorType_Owner : public UGameplayActorType
{
	GENERATED_BODY()
	
protected:
	virtual AActor* TryResolveActor() override;	
};

UCLASS(DisplayName="Level Actor")
class GAMEPLAYFLOW_API UGameplayActorType_LevelActor : public UGameplayActorType
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Actor")
	TSoftObjectPtr<AActor> TargetActor;

	// TODO: Hook into dynamic spawning in
	
protected:
	virtual AActor* TryResolveActor() override;
};

// TODO: Instigator

// TODO: By class
	
/**
 * 
 */
UCLASS(abstract)
class GAMEPLAYFLOW_API UGameplayFlowAbilityCondition : public UGameplayFlowCondition
{
	GENERATED_BODY()

public:	
	/* Actor target to query */
	UPROPERTY(EditAnywhere, Instanced)
	TObjectPtr<UGameplayActorType> ActorTarget;

	UFUNCTION(BlueprintPure)
	AActor* GetTargetActor() const;

protected:
	virtual void ExecuteInput(const FName& PinName) override;
	virtual void Cleanup() override;
	virtual void BindActorEvents();
	/* Called when the actor is removed or the event condition is cleaned up */
	virtual void CleanUpActorEvents();
	virtual bool EvaluatePredicate_Implementation() const override;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
};

UCLASS(DisplayName="Tag Criteria")
class GAMEPLAYFLOW_API UGameplayFlowTagsCondition : public UGameplayFlowAbilityCondition
{
	GENERATED_BODY()

public:	
	/* Tag criteria for actor */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Actor", meta = (DisplayAfter="ActorType"))
	FGameplayTagRequirements TagRequirements;

protected:
	virtual void BindActorEvents() override;
	virtual void CleanUpActorEvents() override;
	
	virtual bool EvaluatePredicate_Implementation() const override;
	FGameplayTagContainer GetAllConditionalTags() const;

	UFUNCTION()
	void OnTagsChanged(const FGameplayTag Tag, int32 Count);
};
