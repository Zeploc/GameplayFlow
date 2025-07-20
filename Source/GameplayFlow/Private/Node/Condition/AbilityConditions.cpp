// Fill out your copyright notice in the Description page of Project Settings.


#include "Node/Condition/AbilityConditions.h"
#include "Node/GameplayActorType.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "FlowAsset.h"
#include "GameplayFlowLogs.h"


AActor* UGameplayFlowAbilityCondition::GetTargetActor() const
{
	if (!ActorTarget)
	{
		GAMEPLAY_FLOW_LOG_WARNING("Actor target null for %s", *GetName());
		return nullptr;
	}
	return ActorTarget->GetActor();
}

void UGameplayFlowAbilityCondition::ExecuteInput(const FName& PinName)
{
	Super::ExecuteInput(PinName);
	
	if (ActorTarget)
	{
		ActorTarget->Register();
	}
	else
	{		
		GAMEPLAY_FLOW_LOG_WARNING("Actor target null for %s", *GetName());
	}
	BindActorEvents();
}

void UGameplayFlowAbilityCondition::Cleanup()
{
	Super::Cleanup();
	CleanUpActorEvents();
	if (ActorTarget)
	{
		ActorTarget->CleanUp();
	}
	else
	{		
		GAMEPLAY_FLOW_LOG_WARNING("Actor target null for %s", *GetName());
	}
}

void UGameplayFlowAbilityCondition::BindActorEvents()
{
	if (!ActorTarget)
	{
		GAMEPLAY_FLOW_LOG_WARNING("Actor target null for %s", *GetName());
		return;
	}
	AActor* Actor = ActorTarget->GetActor();
	if (!Actor)
	{
		return;
	}
	IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(Actor);
	if (!AbilitySystemInterface)
	{
		return;
	}
	
	AbilitySystemComponent = AbilitySystemInterface->GetAbilitySystemComponent();
	K2_BindActorEvents();
}

void UGameplayFlowAbilityCondition::CleanUpActorEvents()
{
	
}

bool UGameplayFlowAbilityCondition::EvaluatePredicate_Implementation() const
{
	if (!AbilitySystemComponent)
	{
		return false;
	}
	return true;
}


#if WITH_EDITORONLY_DATA
void UGameplayFlowAbilityCondition::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	// TODO: Verify works
	if (!ActorTarget)
	{
		EObjectFlags Flags = RF_Public;
		Flags = Flags | (GetOuter() && GetOuter()->HasAnyFlags(RF_ClassDefaultObject) ? RF_ArchetypeObject : RF_NoFlags);
		ActorTarget = NewObject<UGameplayActorType_Owner>(this, UGameplayActorType_Owner::StaticClass(), NAME_None, Flags);
	}
}

FText UGameplayFlowAbilityCondition::GetNodeTitle() const
{
	if (!OverrideDisplayName.IsEmptyOrWhitespace())
	{
		return OverrideDisplayName;
	}
	return Super::GetNodeTitle();
}

#endif

void UGameplayFlowTagsCondition::BindActorEvents()
{
	Super::BindActorEvents();
	if (!AbilitySystemComponent)
	{
		return;
	}
	const FGameplayTagContainer AllTags = GetAllConditionalTags();	
	for (const FGameplayTag& GameplayTag : AllTags)
	{
		AbilitySystemComponent->RegisterGameplayTagEvent(GameplayTag, EGameplayTagEventType::AnyCountChange).AddUObject(this, &UGameplayFlowTagsCondition::OnTagsChanged);
	}
	if (Execute_EvaluatePredicate(this))
	{
		OnEventConditionChanged.Broadcast(this);
	}
}

void UGameplayFlowTagsCondition::CleanUpActorEvents()
{
	Super::CleanUpActorEvents();
	
	if (AbilitySystemComponent)
	{
		const FGameplayTagContainer AllTags = GetAllConditionalTags();	
		for (const FGameplayTag& GameplayTag : AllTags)
		{
			AbilitySystemComponent->RegisterGameplayTagEvent(GameplayTag, EGameplayTagEventType::AnyCountChange).RemoveAll(this);
		}
	}
}

void UGameplayFlowTagsCondition::OnTagsChanged(const FGameplayTag Tag, int32 Count)
{
	OnEventConditionChanged.Broadcast(this);
}

FGameplayTagContainer UGameplayFlowTagCriteriaCondition::GetAllConditionalTags() const
{
	FGameplayTagContainer AllTags = FGameplayTagContainer::CreateFromArray(TagRequirements.TagQuery.GetGameplayTagArray());
	AllTags.AppendTags(TagRequirements.RequireTags);
	AllTags.AppendTags(TagRequirements.IgnoreTags);
	return AllTags;
}

bool UGameplayFlowTagCriteriaCondition::EvaluatePredicate_Implementation() const
{
	if (!Super::EvaluatePredicate_Implementation())
	{
		return false;
	}
		
	return TagRequirements.RequirementsMet(AbilitySystemComponent->GetOwnedGameplayTags());
}

FGameplayTagContainer UGameplayFlowTagCountCondition::GetAllConditionalTags() const
{
	return Tags;
}

bool UGameplayFlowTagCountCondition::EvaluatePredicate_Implementation() const
{
	if (!Super::EvaluatePredicate_Implementation())
	{
		return false;
	}
	for (const FGameplayTag& Tag : Tags)
	{
		const int32 TagCount = AbilitySystemComponent->GetTagCount(Tag);
		if (TagCount < RequiredTagCount)
		{
			return false;
		}
		if (TagCount != RequiredTagCount && bExactOnly)
		{
			return false;
		}
	}
	return true;
}
