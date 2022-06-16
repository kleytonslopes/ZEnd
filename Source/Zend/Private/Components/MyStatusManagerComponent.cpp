// Copyright Sunydark 2022.


#include "Components/MyStatusManagerComponent.h"
#include "Components/MyStatusComponent.h"
#include "MyCharacter.h"

UMyStatusManagerComponent::UMyStatusManagerComponent()
{
}


void UMyStatusManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	ConfigureOwnerEvents();
}

void UMyStatusManagerComponent::ConfigureOwnerEvents()
{
	AMyCharacter* MyCharacter = Cast<AMyCharacter>(GetOwner());

	if (MyCharacter)
	{
		MyCharacter->OnIsWalkingSignature.AddDynamic(this, &UMyStatusManagerComponent::OnMyOwnerIsWalking);
		MyCharacter->OnIsRunningSignature.AddDynamic(this, &UMyStatusManagerComponent::OnMyOwnerIsRunning);
	}
}

void UMyStatusManagerComponent::OnHealthChangedEvent(float Percent)
{
	OnHealthChangedSignature.Broadcast(Percent);
}

void UMyStatusManagerComponent::OnHealthInDangerZoneEvent(bool IsInDangerZone)
{
	OnHealthInDangerZoneSignature.Broadcast(IsInDangerZone);
}

void UMyStatusManagerComponent::OnThristChangedEvent(float Percent)
{
	OnThristChangedSignature.Broadcast(Percent);
}

void UMyStatusManagerComponent::OnThristInDangerZoneEvent(bool IsInDangerZone)
{
	OnThristInDangerZoneSignature.Broadcast(IsInDangerZone);

	if (IsInDangerZone && !bDrainingHealth)
	{
		CheckDangerZones();
		if (bDrainingHealth)
		{
			HealthComponent->SetConsumeByTime(true);
			HealthComponent->StartConsumeByTime();
		}
	}
}

void UMyStatusManagerComponent::SetHealthComponent(UMyStatusComponent* MyStatusComponent)
{
	HealthComponent = MyStatusComponent;

	HealthComponent->OnStatusValueChangedSignature.AddDynamic(this, &UMyStatusManagerComponent::OnHealthChangedEvent);
	HealthComponent->OnStatusValueInDangerZoneSignature.AddDynamic(this, &UMyStatusManagerComponent::OnHealthInDangerZoneEvent);
}

void UMyStatusManagerComponent::SetThirstComponent(UMyStatusComponent* MyStatusComponent)
{
	ThirstComponent = MyStatusComponent;

	ThirstComponent->OnStatusValueChangedSignature.AddDynamic(this, &UMyStatusManagerComponent::OnThristChangedEvent);
	ThirstComponent->OnStatusValueInDangerZoneSignature.AddDynamic(this, &UMyStatusManagerComponent::OnThristInDangerZoneEvent);
}

void UMyStatusManagerComponent::OnMyOwnerIsWalking(bool IsWalking)
{
	HealthComponent->SetIsWalking(IsWalking);
	ThirstComponent->SetIsWalking(IsWalking);
}

void UMyStatusManagerComponent::OnMyOwnerIsRunning(bool IsRunning)
{
	HealthComponent->SetIsRunning(IsRunning);
	ThirstComponent->SetIsRunning(IsRunning);
}

void UMyStatusManagerComponent::CheckDangerZones()
{
	bDrainingHealth = ThirstComponent->GetIfInDangerZone();
}
