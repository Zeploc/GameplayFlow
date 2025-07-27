// Copyright Skyward Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameplayFlowRegistrySubsystem.generated.h"

class UGameplayFlowRegistry;
class IGameplayFlowRegistryInterface;

/**
 * Handles the Registry object
 */
UCLASS(Config = Game)
class GAMEPLAYFLOW_API UGameplayFlowRegistrySubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UPROPERTY(Config)
	TSoftClassPtr<UGameplayFlowRegistry> DefaultRegistryClass;

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
