// Copyright Sunydark 2022.


#include "Components/MyInventoryComponent.h"
#include "Engine/DataTable.h"
#include "UObject/ConstructorHelpers.h"
#include "../Zend.h"

UMyInventoryComponent::UMyInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UDataTable> ItemDataTableObject(TEXT(DATA_ITEM));

	if(ItemDataTableObject.Succeeded()) ItemDataTable = ItemDataTableObject.Object;
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
