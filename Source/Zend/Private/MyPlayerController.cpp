// Copyright Sunydark 2022.


#include "MyPlayerController.h"
#include "MyCharacter.h"
#include "Camera/MyPlayerCameraManager.h"


AMyPlayerController::AMyPlayerController()
{
	PlayerCameraManagerClass = AMyPlayerCameraManager::StaticClass();
}

AMyCharacter* AMyPlayerController::GetMyCharacterRef() const
{
	AMyCharacter* myCharacter = Cast<AMyCharacter>(GetPawn());

	if(myCharacter)
		return myCharacter;

	return nullptr;
}


void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();
}