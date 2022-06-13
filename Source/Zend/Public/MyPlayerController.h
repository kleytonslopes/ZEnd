// Copyright Sunydark 2022.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

class AMyCharacter;

UCLASS()
class ZEND_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	AMyCharacter* GetMyCharacterRef() const;

public:
	AMyPlayerController();

	virtual void BeginPlay() override;
};
