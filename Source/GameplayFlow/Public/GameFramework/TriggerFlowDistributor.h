// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayFlowDistributor.h"
#include "TriggerFlowDistributor.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAMEPLAYFLOW_API UTriggerFlowDistributor : public UGameplayFlowDistributor
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UTriggerFlowDistributor();

	UFUNCTION(BlueprintCallable)
	void AddTriggerComponent(UPrimitiveComponent* TriggerComponent);
	UFUNCTION(BlueprintCallable)
	void RemoveTriggerComponent(UPrimitiveComponent* TriggerComponent);
	UFUNCTION(BlueprintCallable)
	void SetTriggerComponent(UPrimitiveComponent* TriggerComponent);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Gameplay Flow")
	FName EndTriggerEventName = FName("EndTrigger");

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	virtual void ActorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void ActorEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(BlueprintreadOnly)
	TSet<UPrimitiveComponent*> TriggerComponents;
};
