// Copyright Sunydark 2022.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

class AMyCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangedPercentSignature, float, Percent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnThirstChangedPercentSignature, float, Percent);

UCLASS()
class ZEND_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable)
	FOnHealthChangedPercentSignature OnHealthChangedPercentSignature;

	UPROPERTY(BlueprintAssignable)
	FOnThirstChangedPercentSignature OnThirstChangedPercentSignature;

protected:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	AMyCharacter* GetMyCharacterRef() const;

public:
	AMyPlayerController();

	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* aPawn) override;

	UFUNCTION(BlueprintImplementableEvent)
	void OnCharacterHealthChanged(float Percent);
	UFUNCTION(BlueprintImplementableEvent)
	void OnCharacterHealthInDangerZoneChanged(bool IsInDangerZone);

	UFUNCTION(BlueprintImplementableEvent)
	void OnCharacterThirstChanged(float Percent);
	UFUNCTION(BlueprintImplementableEvent)
	void OnCharacterThirstInDangerZoneChanged(bool IsInDangerZone);

	UFUNCTION(BlueprintImplementableEvent)
	void ConfigureHud();

private:
	void ConfigureCharacterEvents();
};
