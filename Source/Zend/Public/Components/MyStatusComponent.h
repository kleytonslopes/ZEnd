// Copyright Sunydark 2022.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MyStatusComponent.generated.h"

USTRUCT(BlueprintType)
struct FStatusMultipliers
{
	GENERATED_BODY()

public:
	FStatusMultipliers()
	{
		WhenWalking = 0.1f;
		WhenRunning = 0.1f;
		WhenCrouching = 0.1f;
	}

	UPROPERTY(EditDefaultsOnly)
	float WhenWalking = 0.1f;

	UPROPERTY(EditDefaultsOnly)
	float WhenRunning = 0.1f;

	UPROPERTY(EditDefaultsOnly)
	float WhenCrouching = 0.1f;
};

UENUM(BlueprintType)
enum EStatusMode
{
	SM_UpDown UMETA(DisplayName = "Up to Down"),
	SM_DownUp UMETA(DisplayName = "Down to Up")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam (FOnStatusValueChangedSignature, float, Percent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam (FOnStatusValueInDangerZoneSignature, bool, IsInDangerZone);
DECLARE_DYNAMIC_MULTICAST_DELEGATE (FOnAlertIfOnLimitSignature);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ZEND_API UMyStatusComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	bool bIsWalking;
	bool bIsRunning;

public:
	UPROPERTY(BlueprintAssignable)
	FOnStatusValueChangedSignature OnStatusValueChangedSignature;

	UPROPERTY(BlueprintAssignable)
	FOnStatusValueInDangerZoneSignature OnStatusValueInDangerZoneSignature;

	UPROPERTY(BlueprintAssignable)
	FOnAlertIfOnLimitSignature OnAlertIfOnLimitSignature;

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FStatusMultipliers StatusMultipliers;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	bool bIsOnLimit = true;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	bool bConsumeByTime = true;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float TickToConsume = 0.1f;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float CurrentValue;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float MaxValue = 500.f;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float ValueToChangeByTick = 0.1f;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float DangerPercentage = 0.25f;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TEnumAsByte<EStatusMode> Mode = EStatusMode::SM_UpDown;

	FTimerHandle HandleTimeToConsume;
public:	
	UMyStatusComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void ResetValue();

	UFUNCTION(BlueprintCallable)
	void ChangeValueNatural();

	UFUNCTION(BlueprintCallable)
	void ChangeValueByValue(float Value);

	UFUNCTION(BlueprintCallable)
	void IncreaseValue(float Value);

	UFUNCTION(BlueprintCallable)
	void DecreaseValue(float Value);

	UFUNCTION(BlueprintCallable)
	void StartConsumeByTime();
	UFUNCTION(BlueprintCallable)
	void StopConsumeByTime();

	UFUNCTION(BlueprintCallable)
	void HandleConsuming();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetPercent() const;
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool GetIfInDangerZone() const;

	UFUNCTION(BlueprintCallable)
	void SetConsumeByTime(bool ConsumeByTime);
	UFUNCTION(BlueprintCallable)
	void SetIsWalking(bool IsWalking);
	UFUNCTION(BlueprintCallable)
	void SetIsRunning(bool IsRunning);

	UFUNCTION(BlueprintCallable)
	float GetCurrentValue() const;
private:
	void CheckIfInDangerZone();
	void AlertIfOnLimit();
	float GetValueToMultiply() const;
};
