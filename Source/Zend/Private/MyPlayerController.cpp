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

	if (myCharacter)
		return myCharacter;

	return nullptr;
}


void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if(IsLocalController())
		ConfigureHud();
}

void AMyPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	ConfigureCharacterEvents();
}

void AMyPlayerController::ConfigureCharacterEvents()
{
	AMyCharacter* MyCharacter = GetMyCharacterRef();

	if (MyCharacter)
	{
		MyCharacter->OnMyHealthChangedSignature.AddDynamic(this, &AMyPlayerController::OnCharacterHealthChanged);
		MyCharacter->OnMyHealthInDangerZoneSignature.AddDynamic(this, &AMyPlayerController::OnCharacterHealthInDangerZoneChanged);

		MyCharacter->OnMyThirstChangedSignature.AddDynamic(this, &AMyPlayerController::OnCharacterThirstChanged);
		MyCharacter->OnMyThirstInDangerZoneSignature.AddDynamic(this, &AMyPlayerController::OnCharacterThirstInDangerZoneChanged);
	}
}
