// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GameplayFlowRegistry.generated.h"

UINTERFACE()
class UGameplayFlowRegistryInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface for gameplay flow registry lifecycle management.
 *
 * Declares methods for initializing and cleaning up the registry. Implementations should provide logic to set up and tear down any resources or state required for gameplay flow registration.
 */
class IGameplayFlowRegistryInterface
{
	GENERATED_BODY()

public:	
	virtual bool Initialize() = 0;
	virtual bool Cleanup() = 0;	
};

/**
 * The main manager for registering actors and linking to them
 */
UCLASS()
class GAMEPLAYFLOW_API UGameplayFlowRegistry : public UObject, public IGameplayFlowRegistryInterface
{
	GENERATED_BODY()

public:
	/**
	 * @brief Initializes the gameplay flow registry.
	 *
	 * @return Returns true to indicate successful initialization.
	 */
	virtual bool Initialize() override;
	
	/**
	 * @brief Performs cleanup operations for the gameplay flow registry.
	 *
	 * @return Returns true to indicate successful cleanup.
	 */
	virtual bool Cleanup() override;
};
