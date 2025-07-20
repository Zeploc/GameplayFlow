// Fill out your copyright notice in the Description page of Project Settings.


#include "Node/GameplayActorNode.h"

#include "GameplayFlowLogs.h"
#include "Node/GameplayActorType.h"

AActor* UGameplayActorNode::GetTargetActor() const
{
	if (!ActorTarget)
	{
		GAMEPLAY_FLOW_LOG_WARNING("Actor target null for %s", *GetName());
		return nullptr;
	}
	return ActorTarget->GetActor();
}

void UGameplayActorNode::ExecuteInput(const FName& PinName)
{	
	if (ActorTarget)
	{
		ActorTarget->Register();
	}
	else
	{
		GAMEPLAY_FLOW_LOG_WARNING("Actor target null for %s", *GetName());
	}
	Super::ExecuteInput(PinName);
}

void UGameplayActorNode::Cleanup()
{
	Super::Cleanup();
	if (ActorTarget)
	{
		ActorTarget->CleanUp();
	}
	else
	{		
		GAMEPLAY_FLOW_LOG_WARNING("Actor target null for %s", *GetName());
	}
}

#if WITH_EDITORONLY_DATA
void UGameplayActorNode::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if (!ActorTarget)
	{
		EObjectFlags Flags = RF_Public;
		Flags = Flags | (GetOuter() && GetOuter()->HasAnyFlags(RF_ClassDefaultObject) ? RF_ArchetypeObject : RF_NoFlags);
		ActorTarget = NewObject<UGameplayActorType_Owner>(this, UGameplayActorType_Owner::StaticClass(), NAME_None, Flags);
	}
}

#endif

