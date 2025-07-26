// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Volume.h"
#include "GameplayFlowVolume.generated.h"

class UTriggerFlowDistributor;

UCLASS()
class GAMEPLAYFLOW_API AGameplayFlowVolume : public AVolume
{
	GENERATED_BODY()

public:
	AGameplayFlowVolume();

	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Gameplay Flow")
	TObjectPtr<UTriggerFlowDistributor> FlowDistributorComponent;	
};

