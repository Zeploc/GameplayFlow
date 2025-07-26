// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Nodes/FlowNode.h"
#include "GameplayActorNode.generated.h"

class UGameplayActorType;
/**
 * 
 */
UCLASS(Blueprintable)
class GAMEPLAYFLOW_API UGameplayActorNode : public UFlowNode
{
	GENERATED_BODY()
	
public:
	/* Actor target to query */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced)
	TObjectPtr<UGameplayActorType> ActorTarget;

	UFUNCTION(BlueprintPure)
	AActor* GetTargetActor() const;
	virtual void ExecuteInput(const FName& PinName) override;
	virtual void Cleanup() override;

#if WITH_EDITORONLY_DATA
	virtual void PostLoad() override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	// TODO: End execute pin
};
