// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FlowComponent.h"
#include "GameplayFlowComponent.generated.h"


UCLASS(ClassGroup=(GameplayFlow), meta=(BlueprintSpawnableComponent))
class GAMEPLAYFLOW_API UGameplayFlowComponent : public UFlowComponent
{
	GENERATED_BODY()

public:
	UGameplayFlowComponent();

	// TODO: Specific event to fire? For shared flow assets

	// TODO: Inline/instanced option (hide owning flow asset?) or set to this instance and clicking on that displays graph?

protected:
	virtual void BeginPlay() override;
};
