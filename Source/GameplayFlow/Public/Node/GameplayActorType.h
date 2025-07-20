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

	UFUNCTION(BlueprintPure, Category="Gameplay Actor Type")
	virtual FString GetNodeDisplay() const;
	UFUNCTION(BlueprintPure, Category="Gameplay Actor Type")
	virtual FString GetDebugDisplay() const;

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

UCLASS(DisplayName="Level Actor")
class GAMEPLAYFLOW_API UGameplayActorType_LevelActor : public UGameplayActorType
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Actor")
	TSoftObjectPtr<AActor> TargetActor;
	
	virtual FString GetNodeDisplay() const override;

	// TODO: Hook into dynamic spawning in
	
protected:
	virtual AActor* TryResolveActor() override;
};

// TODO: Instigator Still needed??

// TODO: By class


// UCLASS(BlueprintType, editinlinenew, collapseCategories, meta = (DisplayName = "Tag Count Match"))
// class UEditableGameplayTagQueryExpression_TagCountMatch : public UEditableGameplayTagQueryExpression
// {
// 	GENERATED_BODY()
// public:
// 	UPROPERTY(EditDefaultsOnly, Category = Expr)
// 	FGameplayTagContainer Tags;
// 	UPROPERTY(EditDefaultsOnly, Category = Expr)
// 	int32 RequiredTagCount;
// 	UPROPERTY(EditDefaultsOnly, Category = Expr)
// 	bool bExactOnly;
//
// #if WITH_EDITOR
// 	virtual void EmitTokens(TArray<uint8>& TokenStream, TArray<FGameplayTag>& TagDictionary, FString* DebugString = nullptr) const override;
// #endif  // WITH_EDITOR
// };