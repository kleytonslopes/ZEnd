// Copyright Sunydark 2022.


#include "Components/MyInventoryManagerComponent.h"
#include "Components/MyInventoryComponent.h"

UMyInventoryManagerComponent::UMyInventoryManagerComponent()
{
}

void UMyInventoryManagerComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UMyInventoryManagerComponent::SetPrymaryInventory(UMyInventoryComponent* PrimaryInventoryRef)
{
	PrimaryInventory = PrimaryInventoryRef;
}

void UMyInventoryManagerComponent::SetSecondaryInventory(UMyInventoryComponent* SecondaryInventoryRef)
{
	SecondaryInventory = SecondaryInventoryRef;
}

void UMyInventoryManagerComponent::TransferAllItensFromTo(UMyInventoryComponent* FromInventory, UMyInventoryComponent* ToInventory)
{
	TArray<FItem> ItensToTransfer = FromInventory->GetItems();

	for (int32 i = 0 ; i < ItensToTransfer.Num() ; i++)
	{
		TransferItemFromTo(ItensToTransfer[i], FromInventory, ToInventory);
	}
}

void UMyInventoryManagerComponent::TransferItemFromTo(FItem ItemToTransfer, UMyInventoryComponent* FromInventory, UMyInventoryComponent* ToInventory)
{
	bool HasItem;
	bool WasRemoved;
	int32 ItemIndex;
	int32 GroupIndex;

	FItem ItemInFromInventory = FromInventory->GetItemByCode(ItemToTransfer.Code, HasItem, GroupIndex, ItemIndex);

	if (ItemInFromInventory.IsValid())
	{
		FromInventory->RemoveItemFromCode(ItemToTransfer.Code, WasRemoved);
		ToInventory->AddItem(ItemInFromInventory);
	}
}
