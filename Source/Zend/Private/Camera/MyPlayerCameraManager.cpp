// Copyright Sunydark 2022.


#include "Camera/MyPlayerCameraManager.h"

AMyPlayerCameraManager::AMyPlayerCameraManager()
{
	NormalFOV = 90.f;

	ViewPitchMax = 87.f;
	ViewPitchMin = -87.f;

	bAlwaysApplyModifiers = true;
}
