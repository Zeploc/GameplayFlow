// Copyright Skyward Studios. All Rights Reserved.


#include "Registry/GameplayFlowRegistrySubsystem.h"

#include "GameplayFlowLogs.h"
#include "Registry/GameplayFlowRegistry.h"

UGameplayFlowRegistrySubsystem::UGameplayFlowRegistrySubsystem()
{
	// Only set if default not already configured
	if (!DefaultRegistryClass)
	{
		DefaultRegistryClass = UGameplayFlowRegistry::StaticClass();
	}
}

void UGameplayFlowRegistrySubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	if (DefaultRegistryClass)
	{
		InitializeRegistry(DefaultRegistryClass.LoadSynchronous());
	}
}

bool UGameplayFlowRegistrySubsystem::InitializeRegistry(const TScriptInterface<IGameplayFlowRegistryInterface>& NewRegistry)
{
	if (!NewRegistry)
	{
		GAMEPLAY_FLOW_LOG_WARNING("Attempted to setup registry null registry!");
		return false;
	}
	if (Registry)
	{
		GAMEPLAY_FLOW_LOG_INFO("Cleaning up existing registry %s before initializing new registry %s", *Registry.GetObject()->GetName(), *NewRegistry.GetObject()->GetName());
		DeinitializeRegistry();
	}
	Registry = NewRegistry;
	const bool bInitialize = Registry->Initialize();
	if (!bInitialize)
	{
		GAMEPLAY_FLOW_LOG_ERROR("Failed to initialize registry %s", *Registry.GetObject()->GetName());
		return false;
	}
	GAMEPLAY_FLOW_LOG_INFO("Initialized registry %s", *Registry.GetObject()->GetName());
	return bInitialize;
}

bool UGameplayFlowRegistrySubsystem::InitializeRegistry(const UClass* RegistryClass)
{
	if (!RegistryClass)
	{
		GAMEPLAY_FLOW_LOG_WARNING("Attempted to setup registry null registry class!");
		return false;
	}
	if (!RegistryClass->ImplementsInterface(UGameplayFlowRegistryInterface::StaticClass()))
	{
		GAMEPLAY_FLOW_LOG_WARNING("Can't initialize registry of class %s, it does not implement %s!", *RegistryClass->GetName(), *UGameplayFlowRegistryInterface::StaticClass()->GetName());
		return false;
	}
	const TObjectPtr<UObject> NewRegistry = NewObject<UObject>(this, RegistryClass);
	return InitializeRegistry(NewRegistry);
}

bool UGameplayFlowRegistrySubsystem::DeinitializeRegistry()
{
	if (!Registry)
	{
		GAMEPLAY_FLOW_LOG_INFO("No registry to deinitialize!")
		return false;
	}
	const bool bCleanupSuccess = Registry->Cleanup();
	if (bCleanupSuccess)
	{
		GAMEPLAY_FLOW_LOG_INFO("Cleaned up existing registry %s", *Registry.GetObject()->GetName());		
	}
	else
	{
		GAMEPLAY_FLOW_LOG_WARNING("Cleaning up registry %s failed!", *Registry.GetObject()->GetName());
	}
	Registry = nullptr;
	return bCleanupSuccess;
}
