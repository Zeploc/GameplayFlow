// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Volume.h"
#include "GameplayFlowVolume.generated.h"

class UGameplayFlowDistributor;

UCLASS()
class GAMEPLAYFLOW_API AGameplayFlowVolume : public AVolume
{
	GENERATED_BODY()

public:
	AGameplayFlowVolume();

	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Gameplay Flow")
	TObjectPtr<UGameplayFlowDistributor> FlowDistributorComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Gameplay Flow")
	FName EndTriggerEventName = FName("EndTrigger");

protected:
	UFUNCTION()
	virtual void ActorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void ActorEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
};

