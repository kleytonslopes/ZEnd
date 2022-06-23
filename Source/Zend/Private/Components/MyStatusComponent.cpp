// Copyright Sunydark 2022.


#include "Components/MyStatusComponent.h"

UMyStatusComponent::UMyStatusComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UMyStatusComponent::BeginPlay()
{
	Super::BeginPlay();
	ResetValue();
	StartConsumeByTime();
}

void UMyStatusComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UMyStatusComponent::ResetValue()
{
	switch (Mode)
	{
	case EStatusMode::SM_UpDown:
		CurrentValue = MaxValue;
		bIsOnLimit = false;
		break;
	case EStatusMode::SM_DownUp:
		CurrentValue = 0.f;
		bIsOnLimit = false;
		break;
	default:
		break;
	}

	OnStatusValueChangedSignature.Broadcast((((CurrentValue * 100.f) / MaxValue) / 100.f));
	CheckIfInDangerZone();
}

void UMyStatusComponent::ChangeValueNatural()
{
	switch (Mode)
	{
	case EStatusMode::SM_UpDown:
		DecreaseValue(ValueToChangeByTick);
		break;
	case EStatusMode::SM_DownUp:
		IncreaseValue(ValueToChangeByTick);
		break;
	default:
		break;
	}
}

void UMyStatusComponent::IncreaseValue(float Value)
{
	if (Value > 0.f)
		CurrentValue = FMath::Clamp((CurrentValue + (Value + GetValueToMultiply())), 0.f, MaxValue);

	OnStatusValueChangedSignature.Broadcast(GetPercent());
	CheckIfInDangerZone();
	AlertIfOnLimit();
}

void UMyStatusComponent::DecreaseValue(float Value)
{
	if (Value > 0.f)
		CurrentValue = FMath::Clamp((CurrentValue - (Value + GetValueToMultiply())), 0.f, MaxValue);

	OnStatusValueChangedSignature.Broadcast(GetPercent());
	CheckIfInDangerZone();
	AlertIfOnLimit();
}

void UMyStatusComponent::StartConsumeByTime()
{
	if (bConsumeByTime)
	{
		GetWorld()->GetTimerManager().SetTimer(HandleTimeToConsume, this, &UMyStatusComponent::HandleConsuming, TickToConsume, true, TickToConsume);
	}
}

void UMyStatusComponent::StopConsumeByTime()
{
	GetWorld()->GetTimerManager().ClearTimer(HandleTimeToConsume);
}

void UMyStatusComponent::HandleConsuming()
{
	ChangeValueNatural();
}

float UMyStatusComponent::GetPercent() const
{
	return (((CurrentValue * 100.f) / MaxValue) / 100.f);
}

bool UMyStatusComponent::GetIfInDangerZone() const
{
	switch (Mode)
	{
	case EStatusMode::SM_UpDown:
		if (GetPercent() <= DangerPercentage)
			return true;
		else
			return false;
		break;
	case EStatusMode::SM_DownUp:
		if (GetPercent() >= DangerPercentage)
			return true;
		else
			return false;
		break;
	default:
		break;
	}
	return false;
}

void UMyStatusComponent::SetConsumeByTime(bool ConsumeByTime)
{
	bConsumeByTime = ConsumeByTime;
}

void UMyStatusComponent::SetIsWalking(bool IsWalking)
{
	bIsWalking = IsWalking;
}

void UMyStatusComponent::SetIsRunning(bool IsRunning)
{
	bIsRunning = IsRunning;
}

float UMyStatusComponent::GetCurrentValue() const
{
	return CurrentValue;
}

void UMyStatusComponent::CheckIfInDangerZone()
{
	OnStatusValueInDangerZoneSignature.Broadcast(GetIfInDangerZone());
}

void UMyStatusComponent::AlertIfOnLimit()
{
	switch (Mode)
	{
	case EStatusMode::SM_UpDown:
		if (CurrentValue <= 0.f)
		{
			OnAlertIfOnLimitSignature.Broadcast();
			bIsOnLimit = true;
		}
		else
			bIsOnLimit = false;

		break;
	case EStatusMode::SM_DownUp:
		if (CurrentValue >= MaxValue)
		{
			OnAlertIfOnLimitSignature.Broadcast();
			bIsOnLimit = true;
		}
		else
			bIsOnLimit = false;

		break;
	default:
		break;
	}
}

float UMyStatusComponent::GetValueToMultiply() const
{
	float ValueToIncrement = 0.f;

	if (bIsWalking)	ValueToIncrement += StatusMultipliers.WhenWalking;
	if (bIsRunning)	ValueToIncrement += StatusMultipliers.WhenRunning;

	return ValueToIncrement;
}

