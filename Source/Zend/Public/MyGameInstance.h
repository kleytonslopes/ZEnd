// Copyright Sunydark 2022.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Online/FMyOnlineSessionSettings.h"
#include "MyGameInstance.generated.h"

class AMyGameSession;

UCLASS()
class ZEND_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
protected:
	IOnlineSessionPtr SessionInterface;
	TSharedPtr<FMyOnlineSessionSettings> HostSettings;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "MyGame")
	FString MapLobby;

protected:
	virtual void Init() override;
	virtual void OnCreateSessionComplete(FName ServerName, bool Succeeded);

public:
	UFUNCTION(BlueprintCallable, Category = "MyGame|Online")
	void CreateServer(FString InServerName, bool bIsLAN);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "MyGame|Online")
	void CreateMainMenu();

	AMyGameSession* GetGameSession() const;
};
