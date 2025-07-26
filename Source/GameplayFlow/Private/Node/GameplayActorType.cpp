// Fill out your copyright notice in the Description page of Project Settings.

#include "Node/GameplayActorType.h"

#include "FlowAsset.h"
#include "GameFramework/GameplayFlowComponent.h"
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

void UGameplayActorType::TrySetDefaultActorTarget(UObject* Outer, TObjectPtr<UGameplayActorType>& ActorTarget)
{
	if (ActorTarget)
	{
		// Already set
		return;
	}
	if (!IsValid(Outer))
	{
		// Not valid outer
		return;
	}
	EObjectFlags Flags = RF_Public;
	Flags = Flags | (Outer->GetOuter() && Outer->GetOuter()->HasAnyFlags(RF_ClassDefaultObject) ? RF_ArchetypeObject : RF_NoFlags);
	ActorTarget = NewObject<UGameplayActorType_Owner>(Outer, UGameplayActorType_Owner::StaticClass(), NAME_None, Flags);
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

AActor* UGameplayActorType_Instigator::TryResolveActor()
{
	UFlowAsset* FlowAsset = GetTypedOuter<UFlowAsset>();
	if (!FlowAsset)
	{
		return nullptr;
	}
	TWeakObjectPtr<UGameplayFlowComponent> GameplayFlowComponent = FlowAsset->GetOwner<UGameplayFlowComponent>();
	if (!GameplayFlowComponent.IsValid())
	{
		return nullptr;
	}
	return GameplayFlowComponent->GetFlowInstigator();
}

FString UGameplayActorType_LevelActor::GetNodeDisplay() const
{
	FString NodeDisplay = Super::GetNodeDisplay();
	NodeDisplay += ": ";
	if (TargetActor.IsNull())
	{
		return NodeDisplay + "None";
	}
	if (TargetActor.IsValid())
	{
		return NodeDisplay + TargetActor->GetActorLabel();
	}
	return NodeDisplay + TargetActor.GetAssetName() + " (Unloaded)";
}

AActor* UGameplayActorType_LevelActor::TryResolveActor()
{
	return TargetActor.Get();
}

// #if WITH_EDITOR
// void UEditableGameplayTagQueryExpression_TagCountMatch::EmitTokens(TArray<uint8>& TokenStream, TArray<FGameplayTag>& TagDictionary, FString* DebugString) const
// {
// 	TokenStream.Add((uint8)EGameplayTagQueryExprType::NoExprMatch + 1);
//
// 	if (DebugString)
// 	{
// 		DebugString->Append(*FString::Printf(TEXT(" `%i COUNT("), RequiredTagCount));
// 	}
// 	
// 	EmitTagTokens(Tags, TokenStream, TagDictionary, DebugString);
//
// 	if (DebugString)
// 	{
// 		DebugString->Append(TEXT(" )"));
// 	}
// }
// #endif

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

FString UGameplayActorType::GetNodeDisplay() const
{
	return GetClass()->GetDisplayNameText().ToString();
}

FString UGameplayActorType::GetDebugDisplay() const
{
	if (FoundLiveActor)
	{
		return FoundLiveActor->GetName();
	}
	return "None";
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