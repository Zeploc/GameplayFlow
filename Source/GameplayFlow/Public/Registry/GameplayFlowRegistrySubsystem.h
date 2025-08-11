// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameplayFlowRegistrySubsystem.generated.h"

class IGameplayFlowRegistryInterface;

/**
 * Subsystem for managing a gameplay flow registry within the game instance.
 *
 * Provides methods to initialize, retrieve, and deinitialize a registry object implementing the IGameplayFlowRegistryInterface.
 * The registry can be set from an existing interface or instantiated from a specified class.
 *
 */
UCLASS(Config = Game)
class GAMEPLAYFLOW_API UGameplayFlowRegistrySubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UPROPERTY(Config)
	TSoftClassPtr<UObject> DefaultRegistryClass;

	UGameplayFlowRegistrySubsystem();
	
	/**
	 * @brief Initializes the subsystem and sets up the default gameplay flow registry.
	 *
	 * If a default registry class is specified, loads and initializes the registry synchronously.
	 */
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	/**
	 * @brief Initializes the gameplay flow registry with the provided registry instance.
	 *
	 * Replaces any existing registry with the given instance, performing cleanup if necessary.
	 *
	 * @param NewRegistry The registry instance to initialize.
	 * @return true if the new registry is successfully initialized; false otherwise.
	 */
	bool InitializeRegistry(const TScriptInterface<IGameplayFlowRegistryInterface>& NewRegistry);
	
	/**
	 * @brief Initializes the gameplay flow registry using the specified class type.
	 *
	 * Attempts to create and initialize a new registry instance from the provided class.
	 * If the class pointer is null or does not implement the required interface, a warning is logged.
	 *
	 * @param RegistryClass The class type to instantiate as the new registry.
	 * @return true if the registry was successfully initialized; false otherwise.
	 */
	bool InitializeRegistry(const UClass* RegistryClass);

	/**
	 * @brief Deinitializes and cleans up the currently active gameplay flow registry.
	 *
	 * If a registry is set, calls its cleanup method, logs the result, and clears the internal reference.
	 * @return true if the registry was cleaned up successfully; false if no registry was set or cleanup failed.
	 */
	bool DeinitializeRegistry();
	
	TScriptInterface<IGameplayFlowRegistryInterface> GetRegistry() const { return Registry; }

private:
	UPROPERTY()
	TScriptInterface<IGameplayFlowRegistryInterface> Registry;
};
