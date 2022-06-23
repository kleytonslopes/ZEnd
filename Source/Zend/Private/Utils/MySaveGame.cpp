// Copyright Sunydark 2022.


#include "Utils/MySaveGame.h"
#include "MyCharacter.h"
#include "Components/MyStatusComponent.h"
#include "Kismet/GameplayStatics.h"

UMySaveGame::UMySaveGame()
{
	SaveSlotName = TEXT("SavedGame");
	UserIndex = 0;
}

void UMySaveGame::SetPlayerSaveData(AMyCharacter* MyCharacter)
{
	PlayerData.PlayerHealth = MyCharacter->GetHealthComponent()->GetCurrentValue();
	PlayerData.PlayerLocation = MyCharacter->GetActorLocation();
	PlayerData.PlayerRotation = MyCharacter->GetActorRotation();
	PlayerData.PlayerThirst = MyCharacter->GetThirstComponent()->GetCurrentValue();
	PlayerData.PlayerItens = MyCharacter->GetItemsToSaveData();
}

void UMySaveGame::SaveCurrentGame(UWorld* World)
{
	APawn* Pawn = World->GetFirstPlayerController()->GetPawn();
	AMyCharacter* MyCharacter = Cast<AMyCharacter>(Pawn);
	if(MyCharacter)
		SetPlayerSaveData(MyCharacter);

	UGameplayStatics::SaveGameToSlot(this, SaveSlotName, UserIndex);
}

void UMySaveGame::LoadCurrentGame(UWorld* World)
{
	UMySaveGame* thisSave = Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, UserIndex));
	if (thisSave)
	{
		AMyCharacter* MyCharacter = Cast<AMyCharacter>(World->GetFirstPlayerController()->GetPawn());
		if (MyCharacter)
		{
			MyCharacter->SetActorLocation(thisSave->PlayerData.PlayerLocation);
			MyCharacter->SetActorRotation(thisSave->PlayerData.PlayerRotation);
		}
	}
}
