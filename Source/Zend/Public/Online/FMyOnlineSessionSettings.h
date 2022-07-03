// Copyright Sunydark 2022.

#pragma once

#include "OnlineSessionSettings.h"

class AGameMode;
class AMyGameSession;

class FMyOnlineSessionSettings : public FOnlineSessionSettings
{

public:
	
	FMyOnlineSessionSettings(bool bIsLAN = false, bool bIsPresence = false, int32 MaxNumPlayers = 20);
	
	virtual ~FMyOnlineSessionSettings() {}

	void ApplyGameSettings(FOnlineSessionSettings* InSettings, AGameMode* CurrentGame, AMyGameSession* CurrentSession);
};
