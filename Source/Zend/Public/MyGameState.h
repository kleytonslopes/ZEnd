// Copyright Sunydark 2022.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "MyGameState.generated.h"

/**
 * 
 */
UCLASS()
class ZEND_API AMyGameState : public AGameState
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Replicated)
	FString ServerName;
};
