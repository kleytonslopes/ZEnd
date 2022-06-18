// Copyright Sunydark 2022.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MyInteractableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMyInteractableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ZEND_API IMyInteractableInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Interact(AActor* Caller);
};
