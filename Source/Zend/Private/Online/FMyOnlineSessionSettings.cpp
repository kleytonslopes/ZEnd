// Copyright Sunydark 2022.


#include "Online/FMyOnlineSessionSettings.h"
#include "GameFramework/GameMode.h"
#include "MyGameSession.h"
#include "MyGameState.h"
#include "OnlineSubsystem.h"
#include "../Zend.h"

FMyOnlineSessionSettings::FMyOnlineSessionSettings(bool bIsLAN, bool bIsPresence, int32 MaxNumPlayers)
{
	NumPublicConnections = MaxNumPlayers;

	if (NumPublicConnections < 0)
	{
		NumPublicConnections = 0;
	}

	NumPrivateConnections = 4;
	bIsLANMatch = bIsLAN;
	bShouldAdvertise = true;
	bAllowJoinInProgress = true;
	bAllowInvites = true;
	bUsesPresence = bIsPresence;
	bAllowJoinViaPresence = true;
	bAllowJoinViaPresenceFriendsOnly = false;
}

void FMyOnlineSessionSettings::ApplyGameSettings(FOnlineSessionSettings* InSettings, AGameMode* CurrentGame, AMyGameSession* CurrentSession)
{
	if (!CurrentGame)
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid GameMode!"));
		return;
	}
	bool isDedicatedServier = CurrentGame->GetWorld()->GetNetMode() == NM_DedicatedServer;
	InSettings->bIsDedicated = isDedicatedServier;
	InSettings->bIsLANMatch = (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL");

	int32 randInt = FMath::RandRange(100000, 999999);
	FString NewServerName = FString::FromInt(randInt);

	InSettings->Set(SETTING_GAMEMODE, CurrentGame->GetClass()->GetPathName(), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	InSettings->Set(SETTING_MAPNAME, CurrentGame->GetWorld()->GetMapName(), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	InSettings->Set(SETTING_SESSIONKEY, CurrentGame->GetWorld()->GetMapName(), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	InSettings->Set(SETTING_GAMENAME, NewServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	AMyGameState* GameState = CurrentGame->GetGameState<AMyGameState>();
	if (GameState)
	{
		InSettings->Set(SETTING_SERVERNAME, GameState->ServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	}

	InSettings->Set(SETTING_NUMPLAYERSONLINE, CurrentGame->NumPlayers, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	InSettings->Set(SETTING_MAXPLAYERS, CurrentSession->MaxPlayers, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
}
