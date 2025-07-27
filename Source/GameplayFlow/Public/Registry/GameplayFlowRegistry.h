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

class IGameplayFlowRegistryInterface
{
	GENERATED_BODY()

public:	
	virtual bool Initialize() = 0;
	virtual bool Cleanup() = 0;	
};

/**
 * 
 */
UCLASS()
class GAMEPLAYFLOW_API UGameplayFlowRegistry : public UObject, public IGameplayFlowRegistryInterface
{
	GENERATED_BODY()

public:
	virtual bool Initialize() override;
	virtual bool Cleanup() override;
};
