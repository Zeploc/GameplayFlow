// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameplayFlowRegistrySubsystem.generated.h"

class IGameplayFlowRegistryInterface;

/**
 * Handles the Registry object
 */
UCLASS(Config = Game)
class /**
 * Subsystem for managing a gameplay flow registry within the game instance.
 *
 * Provides methods to initialize, retrieve, and deinitialize a registry object implementing the IGameplayFlowRegistryInterface.
 * The registry can be set from an existing interface or instantiated from a specified class.
 *
 * @returns The current gameplay flow registry interface when calling GetRegistry().
 */
GAMEPLAYFLOW_API UGameplayFlowRegistrySubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UPROPERTY(Config)
	TSoftClassPtr<UObject> DefaultRegistryClass;

	UGameplayFlowRegistrySubsystem();
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	bool InitializeRegistry(const TScriptInterface<IGameplayFlowRegistryInterface>& NewRegistry);
	bool InitializeRegistry(const UClass* RegistryClass);

	bool DeinitializeRegistry();
	
	TScriptInterface<IGameplayFlowRegistryInterface> GetRegistry() const { return Registry; }

private:
	UPROPERTY()
	TScriptInterface<IGameplayFlowRegistryInterface> Registry;
};
