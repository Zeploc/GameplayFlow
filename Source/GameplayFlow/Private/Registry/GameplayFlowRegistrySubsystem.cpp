// Fill out your copyright notice in the Description page of Project Settings.


#include "Registry/GameplayFlowRegistrySubsystem.h"

#include "GameplayFlowLogs.h"
#include "Registry/GameplayFlowRegistry.h"

/**
 * @brief Constructs the subsystem and sets the default registry class.
 *
 * Initializes the subsystem with the default gameplay flow registry class.
 */
UGameplayFlowRegistrySubsystem::UGameplayFlowRegistrySubsystem()
{
	// Default initialize with base.. should probably be done elsewhere
	// Change to config set
	DefaultRegistryClass = UGameplayFlowRegistry::StaticClass();
}

/**
 * @brief Initializes the subsystem and sets up the default gameplay flow registry.
 *
 * Calls the superclass initialization and, if a default registry class is specified, loads and initializes the registry synchronously.
 */
void UGameplayFlowRegistrySubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	if (DefaultRegistryClass)
	{
		InitializeRegistry(DefaultRegistryClass.LoadSynchronous());
	}
}

/**
 * @brief Initializes the gameplay flow registry with the provided registry instance.
 *
 * Replaces any existing registry with the given instance, performing cleanup if necessary. Returns true if the new registry is successfully initialized; otherwise, returns false.
 *
 * @param NewRegistry The registry instance to initialize.
 * @return true if initialization succeeds; false otherwise.
 */
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

/**
 * @brief Initializes the gameplay flow registry using the specified class type.
 *
 * Attempts to create and initialize a new registry instance from the provided class. If the class pointer is null or does not implement the required interface, a warning is logged. Returns true if the registry is successfully initialized; otherwise, returns false.
 *
 * @param RegistryClass The class type to instantiate as the new registry.
 * @return true if the registry was successfully initialized; false otherwise.
 */
bool UGameplayFlowRegistrySubsystem::InitializeRegistry(const UClass* RegistryClass)
{
	if (!RegistryClass)
	{
		GAMEPLAY_FLOW_LOG_WARNING("Attempted to setup registry null registry class!");
		return false;
	}
	if (!RegistryClass->ImplementsInterface(UGameplayFlowRegistryInterface::StaticClass()))
	{
		GAMEPLAY_FLOW_LOG_WARNING("Can't initialize registry of class %s, is does not implement %s!", *RegistryClass->GetName(), *UGameplayFlowRegistryInterface::StaticClass()->GetName());
	}
	const TObjectPtr<UObject> NewRegistry = NewObject<UObject>(this, RegistryClass);
	return InitializeRegistry(NewRegistry);
}

/**
 * @brief Deinitializes and cleans up the currently active gameplay flow registry.
 *
 * If a registry is set, calls its cleanup method, logs the result, and clears the internal reference.
 * @return true if the registry was cleaned up successfully; false if no registry was set or cleanup failed.
 */
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
