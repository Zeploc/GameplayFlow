// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "GameplayActorType.generated.h"

UCLASS(Abstract, Blueprintable, EditInlineNew, CollapseCategories)
class GAMEPLAYFLOW_API UGameplayActorType : public UObject
{
	GENERATED_BODY()
	
public:

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnActorFound, UGameplayActorType*, ActorType, AActor*, FoundActor);
	FOnActorFound OnActorFound;
	
	FOnActorFound OnActorLost;

	virtual void Register();
	virtual AActor* GetActor() const;
	virtual void CleanUp();

	UFUNCTION(BlueprintPure, DisplayName="Get Node Display", Category="Gameplay Actor Type")
	FString K2_GetNodeDisplay() const;
	UFUNCTION(BlueprintPure, DisplayName="Get Debug Display", Category="Gameplay Actor Type")
	FString K2_GetDebugDisplay() const;
	
#if WITH_EDITOR
	virtual FString GetNodeDisplay() const;
	virtual FString GetDebugDisplay() const;

	static void TrySetDefaultActorTarget(UObject* Outer, TObjectPtr<UGameplayActorType>& ActorTarget);
#endif
	
protected:
	UPROPERTY()
	TObjectPtr<AActor> FoundLiveActor;
	
	void TryRegisterActor();
	virtual AActor* TryResolveActor();
	void OnActorRegistered();
	void UnregisterActor(AActor* Actor, bool bBroadcastLost);
	
	UFUNCTION()
	void BoundActorEndPlay(AActor* Actor , EEndPlayReason::Type EndPlayReason);

	// TODO:
	UFUNCTION()
	void ItemFound(AActor* RegisteredItem);
};

UCLASS(DisplayName="Player")
class GAMEPLAYFLOW_API UGameplayActorType_Player : public UGameplayActorType
{
	GENERATED_BODY()
	
protected:
	virtual AActor* TryResolveActor() override;	
};

UCLASS(DisplayName="Owner")
class GAMEPLAYFLOW_API UGameplayActorType_Owner : public UGameplayActorType
{
	GENERATED_BODY()
	
protected:
	virtual AActor* TryResolveActor() override;	
};

UCLASS(DisplayName="Instigator")
class GAMEPLAYFLOW_API UGameplayActorType_Instigator : public UGameplayActorType
{
	GENERATED_BODY()
	
protected:
	virtual AActor* TryResolveActor() override;	
};

UCLASS(DisplayName="Level Actor")
class GAMEPLAYFLOW_API UGameplayActorType_LevelActor : public UGameplayActorType
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Actor")
	TSoftObjectPtr<AActor> TargetActor;
	
#if WITH_EDITOR
	virtual FString GetNodeDisplay() const override;
#endif

	// TODO: Hook into dynamic spawning in
	
protected:
	virtual AActor* TryResolveActor() override;
};

// TODO: Instigator Still needed??

// TODO: By class