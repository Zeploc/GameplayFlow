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
 * @returns True if the operation succeeds; false otherwise.
 */
class IGameplayFlowRegistryInterface
{
	GENERATED_BODY()

public:	
	virtual bool Initialize() = 0;
	virtual bool Cleanup() = 0;	
};

/**
 * The main manager for registrering actors and linking to them
 */
UCLASS()
class GAMEPLAYFLOW_API UGameplayFlowRegistry : public UObject, public IGameplayFlowRegistryInterface
{
	GENERATED_BODY()

public:
	virtual bool Initialize() override;
	virtual bool Cleanup() override;
};
