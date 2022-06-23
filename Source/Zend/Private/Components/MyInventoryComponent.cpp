// Copyright Sunydark 2022.


#include "Components/MyInventoryComponent.h"
#include "Engine/DataTable.h"
#include "UObject/ConstructorHelpers.h"
#include "../Zend.h"

UMyInventoryComponent::UMyInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UDataTable> ItemDataTableObject(TEXT(DATA_ITEM));

	if (ItemDataTableObject.Succeeded()) ItemDataTable = ItemDataTableObject.Object;
}

void UMyInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UMyInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UMyInventoryComponent::AddItem(FItem NewItem)
{
	NewItem.GenerateNewCode();
	bool groupFound = false;
	if (ItemGroups.Num())
	{
		for (int32 i = 0; i < ItemGroups.Num(); ++i)
		{
			if (ItemGroups[i].ItemId == NewItem.Id)
			{
				groupFound = true;
				ItemGroups[i].AddItem(NewItem);
			}
		}
	}

	if (!groupFound)
	{
		FItemGroup NewItemGroup;
		NewItemGroup.AddItem(NewItem);

		ItemGroups.Add(NewItemGroup);
	}

	OnInventoryChangedSignature.Broadcast();
}

void UMyInventoryComponent::AddItems(const TArray<FItem>& ItemsToAdd)
{
	int32 totalItens = ItemsToAdd.Num();

	for (int32 i = 0; i < totalItens; i++)
	{
		AddItem(ItemsToAdd[i]);
	}
}

void UMyInventoryComponent::AddItemByName(FName ItemName)
{
	if (ItemDataTable)
	{
		static const FString ContextString(TEXT("Item String Context"));
		const FItem* ItemFound = ItemDataTable->FindRow<FItem>(ItemName, ContextString, true);

		if (ItemFound)
		{
			AddItem(*ItemFound);
		}
	}
}

TArray<FItemGroup> UMyInventoryComponent::GetItemGroups() const
{
	return ItemGroups;
}

TArray<FItem> UMyInventoryComponent::GetItems() const
{
	TArray<FItem> TempItems;

	for (int32 g = 0; g < ItemGroups.Num(); g++)
	{
		for (int32 i = 0; i < ItemGroups[g].Itens.Num(); i++)
		{
			TempItems.Add(ItemGroups[g].Itens[i]);
		}
	}

	return TempItems;
}

FItem UMyInventoryComponent::GetItemByCode(FString ItemCode, bool& HasItem, int32& GroupIndex, int32& ItemIndex)
{
	for (int32 g = 0; g < ItemGroups.Num(); g++)
	{
		for (int32 i = 0; i < ItemGroups[g].Itens.Num(); i++)
		{
			if (ItemGroups[g].Itens[i].Code == ItemCode)
			{
				GroupIndex = g;
				ItemIndex = i;
				HasItem = true;
				return ItemGroups[g].Itens[i];
			}
		}
	}

	HasItem = false;
	return FItem();
}

void UMyInventoryComponent::RemoveItemFromCode(FString ItemCode, bool& WasRemoved)
{
	bool HasItem;
	int32 ItemIndex;
	int32 GroupIndex;

	GetItemByCode(ItemCode, HasItem, GroupIndex, ItemIndex);

	if (HasItem)
	{
		RemoveItem(GroupIndex, ItemIndex);
		WasRemoved = true;
		return;
	}

	WasRemoved = false;
}

void UMyInventoryComponent::RemoveItem(int32 GroupIndex, int32 ItemIndex)
{
	ItemGroups[GroupIndex].Itens.RemoveAt(ItemIndex);
	
	if(ItemGroups[GroupIndex].Itens.Num() == 0)
		ItemGroups.RemoveAt(GroupIndex);

	OnInventoryChangedSignature.Broadcast();
}

TArray<FItemDataSaveGame> UMyInventoryComponent::GetItemsToSaveGame() const
{
	TArray<FItem> ItensTemp = GetItems();
	TArray< FItemDataSaveGame> ItensData;
	for (int32 i = 0; i < ItensTemp.Num(); i++)
	{
		FItemDataSaveGame ItemData;
		ItemData.SetData(ItensTemp[i]);

		ItensData.Add(ItemData);
	}

	return ItensData;
}
