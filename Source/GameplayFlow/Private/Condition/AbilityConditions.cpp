// Fill out your copyright notice in the Description page of Project Settings.


#include "Condition/AbilityConditions.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "FlowAsset.h"
#include "GameplayFlowLogs.h"
#include "Kismet/GameplayStatics.h"

void UGameplayActorType::Register()
{
	// Setup any binding to listen for actor existing
	
	TryRegisterActor();
}

AActor* UGameplayActorType::GetActor() const
{
	return FoundLiveActor.Get();
}

void UGameplayActorType::ItemFound(AActor* RegisteredItem)
{
	TryRegisterActor();
}

AActor* UGameplayActorType_Player::TryResolveActor()
{
	return UGameplayStatics::GetPlayerPawn(this, 0);
}

AActor* UGameplayActorType_Owner::TryResolveActor()
{
	UFlowAsset* FlowAsset = GetTypedOuter<UFlowAsset>();
	if (!FlowAsset)
	{
		return nullptr;
	}
	TWeakObjectPtr<UActorComponent> ActorComponent = FlowAsset->GetOwner<UActorComponent>();
	if (!ActorComponent.IsValid())
	{
		return nullptr;
	}
	return ActorComponent->GetOwner();
}

AActor* UGameplayActorType_LevelActor::TryResolveActor()
{
	return TargetActor.Get();
}

void UGameplayActorType::TryRegisterActor()
{
	if (IsValid(FoundLiveActor.Get()))
	{
		// Already registered
		return;
	}
	FoundLiveActor = TryResolveActor();
	if (!IsValid(FoundLiveActor))
	{
		// Failed to resolve
		return;
	}
	OnActorRegistered();
}

AActor* UGameplayActorType::TryResolveActor()
{
	unimplemented();
	return nullptr;
}

void UGameplayActorType::OnActorRegistered()
{
	FoundLiveActor->OnEndPlay.AddDynamic(this, &UGameplayActorType::BoundActorEndPlay);
	OnActorFound.Broadcast(this, FoundLiveActor);
}

void UGameplayActorType::BoundActorEndPlay(AActor* Actor, EEndPlayReason::Type EndPlayReason)
{
	UnregisterActor(Actor, true);
}

void UGameplayActorType::CleanUp()
{
	UnregisterActor(FoundLiveActor, false);
	// TODO: Unbind actor listen link
}

void UGameplayActorType::UnregisterActor(AActor* Actor, bool bBroadcastLost)
{
	if (!Actor)
	{
		return;
	}
	Actor->OnEndPlay.RemoveDynamic(this, &UGameplayActorType::BoundActorEndPlay);
	if (FoundLiveActor != Actor)
	{
		return;
	}
	FoundLiveActor = nullptr;
	if (bBroadcastLost)
	{
		// Needs to be afterwards so any condition check fails as found live actor is null
		OnActorLost.Broadcast(this, Actor);
	}
}

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

FGameplayTagContainer UGameplayFlowTagsCondition::GetAllConditionalTags() const
{
	FGameplayTagContainer AllTags = FGameplayTagContainer::CreateFromArray(TagRequirements.TagQuery.GetGameplayTagArray());
	AllTags.AppendTags(TagRequirements.RequireTags);
	AllTags.AppendTags(TagRequirements.IgnoreTags);
	return AllTags;
}

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
		AbilitySystemComponent->RegisterGameplayTagEvent(GameplayTag).AddUObject(this, &UGameplayFlowTagsCondition::OnTagsChanged);
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
			AbilitySystemComponent->RegisterGameplayTagEvent(GameplayTag).RemoveAll(this);
		}
	}
}

bool UGameplayFlowTagsCondition::EvaluatePredicate_Implementation() const
{
	if (!Super::EvaluatePredicate_Implementation())
	{
		return false;
	}
		
	return TagRequirements.RequirementsMet(AbilitySystemComponent->GetOwnedGameplayTags());
}

void UGameplayFlowTagsCondition::OnTagsChanged(const FGameplayTag Tag, int32 Count)
{
	OnEventConditionChanged.Broadcast(this);
}
