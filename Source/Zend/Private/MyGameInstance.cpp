// Copyright Sunydark 2022.


#include "MyGameInstance.h"
#include "MyGameMode.h"
//Online
#include "Net/UnrealNetwork.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "MyGameSession.h"
#include "Online/FMyOnlineSessionSettings.h"
//Framework
#include "GameFramework/GameModeBase.h"

void UMyGameInstance::Init()
{
	Super::Init();

	if (IOnlineSubsystem* SubSystem = IOnlineSubsystem::Get())
	{
		SessionInterface = SubSystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UMyGameInstance::OnCreateSessionComplete);
		}
	}
}

void UMyGameInstance::OnCreateSessionComplete(FName ServerName, bool Succeeded)
{
	if (Succeeded)
	{
		GetWorld()->ServerTravel(MapLobby + "?listen");
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("CreateSession Error"));
	}
}

void UMyGameInstance::CreateServer(FString InServerName, bool bIsLAN)
{
	IOnlineSubsystem* const OnlineSubsystem = Online::GetSubsystem(GetWorld());
	AMyGameSession* const GameSession = GetGameSession();
	
	if (OnlineSubsystem)
	{
		IOnlineSessionPtr Sessions = OnlineSubsystem->GetSessionInterface();
		if (Sessions.IsValid())
		{
			HostSettings = MakeShareable(new FMyOnlineSessionSettings(true, true, 20));
			if (HostSettings.IsValid())
			{
				HostSettings->ApplyGameSettings(HostSettings.Get(), GetWorld()->GetAuthGameMode<AGameMode>(), GameSession);
				Sessions->CreateSession(0, FName(InServerName), *HostSettings);
			}
		}
	}
}

AMyGameSession* UMyGameInstance::GetGameSession() const
{
	AGameModeBase* GameModeBase = GetWorld()->GetAuthGameMode();
	if (GameModeBase)
	{
		return Cast<AMyGameSession>(GameModeBase->GameSession);
	}
	return nullptr;
}
