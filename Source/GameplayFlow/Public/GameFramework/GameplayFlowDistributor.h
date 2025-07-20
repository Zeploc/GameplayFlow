// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayFlowDistributor.generated.h"

class UFlowAsset;
class UGameplayFlowComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAMEPLAYFLOW_API UGameplayFlowDistributor : public UActorComponent
{
	GENERATED_BODY()

public:
	UGameplayFlowDistributor();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay Flow")
	UFlowAsset* FlowAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay Flow")
	TSubclassOf<UGameplayFlowComponent> FlowComponentClass;

	// Whether this distributor can add a flow multiple times to the same target before finishing
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay Flow")
	bool bMultipleDistributionInstancesPerTarget = false;

	// Can apply when an existing flow with this asset already exists on the target
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay Flow")
	bool bSkipExistingMatchingFlowAsset = false;

	// TODO: Event to trigger if flow exists on asset?

	// TODO: Global asset boolean?...

	UFUNCTION(BlueprintCallable, Category = "Gameplay Flow")
	void DistributeFlow(AActor* TargetActor);

	UFUNCTION(BlueprintCallable, Category = "Gameplay Flow")
	bool SendEvent(AActor* TargetActor, FName EventName);
	
	UFUNCTION(BlueprintCallable, Category = "Gameplay Flow")
	void RemoveFlow(AActor* TargetActor);

protected:
	/** Returns true if a flow is already applied of the same asset */
	bool HasMatchingFlowAsset(const AActor* TargetActor) const;
	void CleanupInactiveFlows(const AActor* TargetActor);

	TMap<AActor*, TArray<UGameplayFlowComponent*>> FlowComponents;
};
