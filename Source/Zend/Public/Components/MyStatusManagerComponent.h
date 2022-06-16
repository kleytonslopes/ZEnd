// Copyright Sunydark 2022.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../Zend.h"
#include "MyStatusManagerComponent.generated.h"

class UMyStatusComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangedSignature, float, Percent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthInDangerZoneSignature, bool, IsInDangerZone);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnThristChangedSignature, float, Percent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnThristInDangerZoneSignature, bool, IsInDangerZone);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ZEND_API UMyStatusManagerComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	bool bDrainingHealth;
	UMyStatusComponent* HealthComponent;
	UMyStatusComponent* ThirstComponent;

public:
	UPROPERTY(BlueprintAssignable)
	FOnHealthChangedSignature OnHealthChangedSignature;
	UPROPERTY(BlueprintAssignable)
	FOnHealthInDangerZoneSignature OnHealthInDangerZoneSignature;
	
	UPROPERTY(BlueprintAssignable)
	FOnThristChangedSignature OnThristChangedSignature;
	UPROPERTY(BlueprintAssignable)
	FOnThristInDangerZoneSignature OnThristInDangerZoneSignature;

public:	
	UMyStatusManagerComponent();

protected:
	virtual void BeginPlay() override;

public:	
	UFUNCTION()
	void ConfigureOwnerEvents();

	UFUNCTION()
	void OnHealthChangedEvent(float Percent);
	UFUNCTION()
	void OnHealthInDangerZoneEvent(bool IsInDangerZone);
	UFUNCTION()
	void OnThristChangedEvent(float Percent);
	UFUNCTION()
	void OnThristInDangerZoneEvent(bool IsInDangerZone);

	UFUNCTION(BlueprintCallable)
	void SetHealthComponent(UMyStatusComponent* MyStatusComponent);
	UFUNCTION(BlueprintCallable)
	void SetThirstComponent(UMyStatusComponent* MyStatusComponent);

	UFUNCTION()
	void OnMyOwnerIsWalking(bool IsWalking);
	UFUNCTION()
	void OnMyOwnerIsRunning(bool IsRunning);

private:
	void CheckDangerZones();
};
