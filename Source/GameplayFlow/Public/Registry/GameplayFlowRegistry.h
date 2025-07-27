// Copyright Skyward Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GameplayFlowRegistry.generated.h"

UINTERFACE()
class UGameplayFlowRegistryInterface : public UInterface
{
	GENERATED_BODY()
};

class IGameplayFlowRegistryInterface
{
	GENERATED_BODY()

public:	
	virtual bool Initialize() = 0;
	virtual bool Cleanup() = 0;	
};

/**
* * The main manager for registering actors and linking to them.
  * Handles initialization and cleanup of gameplay flow registry functionality.
  * Initialize() should be called before using registry features.
  * Cleanup() should be called to properly release resources.
 */
UCLASS()
class GAMEPLAYFLOW_API UGameplayFlowRegistry : public UObject, public IGameplayFlowRegistryInterface
{
	GENERATED_BODY()

public:
	virtual bool Initialize() override;
	virtual bool Cleanup() override;
};
