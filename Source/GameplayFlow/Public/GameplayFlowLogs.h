// Copyright Skyward Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(LogGameplayFlow, Log, All);

#define GAMEPLAY_FLOW_LOG_WARNING(Format, ...) \
{ \
UE_LOG(LogGameplayFlow, Warning, TEXT(Format"\n[File:%hs] [Line: %i]"), ##__VA_ARGS__, __FILE__, __LINE__); \
GEngine->AddOnScreenDebugMessage(-1, 6.0f, FColor::Orange, FString::Printf(TEXT("Gameplay Flow: " Format), ##__VA_ARGS__));\
}
#define GAMEPLAY_FLOW_LOG_ERROR(Format, ...) \
{ \
UE_LOG(LogGameplayFlow, Error, TEXT(Format"\n[File:%hs] [Line: %i]"), ##__VA_ARGS__, __FILE__, __LINE__); \
GEngine->AddOnScreenDebugMessage(-1, 6.0f, FColor::Red, FString::Printf(TEXT("Gameplay Flow: " Format), ##__VA_ARGS__));\
}