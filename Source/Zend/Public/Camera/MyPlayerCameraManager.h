// Copyright Sunydark 2022.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "MyPlayerCameraManager.generated.h"

/**
 * 
 */
UCLASS()
class ZEND_API AMyPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()
	
public:
	float NormalFOV;

public:
	AMyPlayerCameraManager();
};
